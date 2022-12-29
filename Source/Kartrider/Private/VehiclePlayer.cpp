// Fill out your copyright notice in the Description page of Project Settings.


#include "VehiclePlayer.h"
#include "EnhancedInputSubsystems.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/LocalPlayer.h"
#include "GameFramework/PlayerController.h"
#include "ChaosWheeledVehicleMovementComponent.h"






AVehiclePlayer::AVehiclePlayer()
{
	PrimaryActorTick.bCanEverTick = true;

	// �ڵ����� ���� ���̷�Ż �޽� ������Ʈ ���� �� ��Ʈ ������Ʈ�� ����
	/*VehicleMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("VehicleMeshComponent"));
	RootComponent = VehicleMeshComponent;*/
	
	// ������ �� ������Ʈ ���� �� ��Ʈ ������Ʈ�� ���δ�.
	VehicleSpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("VehicleSpringArmComponent"));
	VehicleSpringArmComponent->SetupAttachment(RootComponent);

	// ī�޶� ������Ʈ ���� �� ������ �� ������Ʈ�� ���δ�.
	VehicleCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("VehicleCameraComponent"));
	VehicleCameraComponent->SetupAttachment(VehicleSpringArmComponent);

	// ĸ�� ī�޶� ���� ������ �� ������Ʈ ���� �� ��Ʈ ������Ʈ�� ���δ�.
	SceneCaptureSpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SceneCaptureSpringArmComponent"));
	SceneCaptureSpringArmComponent->SetupAttachment(RootComponent);

	SceneCaptureCameraComponent = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("SceneCaptureCameraComponent"));
	SceneCaptureCameraComponent->SetupAttachment(SceneCaptureSpringArmComponent);

	// �ν��͸� ���� ������ Thruster ���� �� ��Ʈ ������Ʈ�� ���δ�.
	RightThruster = CreateDefaultSubobject<UPhysicsThrusterComponent>(TEXT("RightBoosterThruster"));
	RightThruster->SetupAttachment(RootComponent);
	RightThruster->SetAutoActivate(false);

	// �ν��͸� ���� ���� Thruster ���� �� ��Ʈ ������Ʈ�� ���δ�.
	LeftThruster = CreateDefaultSubobject<UPhysicsThrusterComponent>(TEXT("LeftBoosterThruster"));
	LeftThruster->SetupAttachment(RootComponent);
	LeftThruster->SetAutoActivate(false);

	// �ν��� ȿ���� ���� ������ Point Light ���� �� ������ Thruster�� ���δ�.
	RightBoosterLight = CreateDefaultSubobject<UPointLightComponent>(TEXT("RightBoosterLight"));
	RightBoosterLight->SetupAttachment(RightThruster);
	RightBoosterLight->ToggleVisibility(false);
	RightBoosterLight->SetCastShadows(false);

	// �ν��� ȿ���� ���� ���� Point Light ���� �� ���� Thruster�� ���δ�.
	LeftBoosterLight = CreateDefaultSubobject<UPointLightComponent>(TEXT("LeftBoosterLight"));
	LeftBoosterLight->SetupAttachment(LeftThruster);
	LeftBoosterLight->ToggleVisibility(false);
	LeftBoosterLight->SetCastShadows(false);

	// ThrottleAction->

	// GetVehicleMovementComponent()->SetupVehicleMass()
}

void AVehiclePlayer::BeginPlay() {
	Super::BeginPlay();

	/*int32 Priority = 0;
	if (ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>(Player))
	{
		if (UEnhancedInputLocalPlayerSubsystem* InputSystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			if (!InputMapping.IsNull())
			{
				InputSystem->AddMappingContext(InputMapping.LoadSynchronous(), Priority);
			}
		}
	}*/

	// ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>(Player);
	APlayerController* con = Cast<APlayerController>(Controller);
	UEnhancedInputLocalPlayerSubsystem* subsys = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(con->GetLocalPlayer());
	subsys->AddMappingContext(BaseMappingContext, 0);

}

void AVehiclePlayer::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

}

void AVehiclePlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	// ����Ʋ �׼� ���ε�
	Input->BindAction(ThrottleAction, ETriggerEvent::Triggered, this, &AVehiclePlayer::EnhancedThrottle);
	Input->BindAction(ThrottleAction, ETriggerEvent::Completed, this, &AVehiclePlayer::EnhancedThrottle);

	// ��Ƽ� �׼� ���ε�
	Input->BindAction(SteeringAction, ETriggerEvent::Triggered, this, &AVehiclePlayer::EnhancedSteering);
	Input->BindAction(SteeringAction, ETriggerEvent::Completed, this, &AVehiclePlayer::EnhancedSteering);

	// �극��ũ �׼� ���ε�
	Input->BindAction(BreakAction, ETriggerEvent::Triggered, this, &AVehiclePlayer::EnhancedBreak);
	Input->BindAction(BreakAction, ETriggerEvent::Started, this, &AVehiclePlayer::EnhancedBreak);
	Input->BindAction(BreakAction, ETriggerEvent::Completed, this, &AVehiclePlayer::EnhancedBreak);

	// �帮��Ʈ �׼� ���ε�
	Input->BindAction(DriftAction, ETriggerEvent::Triggered, this, &AVehiclePlayer::EnhancedDrift);
	Input->BindAction(DriftAction, ETriggerEvent::Started, this, &AVehiclePlayer::EnhancedDrift);
	Input->BindAction(DriftAction, ETriggerEvent::Ongoing, this, &AVehiclePlayer::EnhancedDrift);
	Input->BindAction(DriftAction, ETriggerEvent::Completed, this, &AVehiclePlayer::EnhancedDriftCompleted);

	// ���� �׼� ���ε�
	Input->BindAction(ResetAction, ETriggerEvent::Triggered, this, &AVehiclePlayer::EnhancedReset);
	
	// �ν��� �׼� ���ε�
	Input->BindAction(BoosterAction, ETriggerEvent::Triggered, this, &AVehiclePlayer::EnhancedBooster);
	// Input->BindAction(BoosterAction, ETriggerEvent::Completed, this, &AVehiclePlayer::EnhancedBoosterCompleted);

}

void AVehiclePlayer::EnhancedThrottle(const FInputActionValue& Value) {
	
	GetVehicleMovementComponent()->SetThrottleInput(Value.GetMagnitude());
	// GetVehicleMovementComponent()->SetThrottleInput(Value.Get<float>());
}

void AVehiclePlayer::EnhancedSteering(const FInputActionValue& Value) {
	
	GetVehicleMovementComponent()->SetSteeringInput(Value.GetMagnitude());
}

void AVehiclePlayer::EnhancedBreak(const FInputActionValue& Value) {
	
	GetVehicleMovementComponent()->SetBrakeInput(Value.GetMagnitude());
	// UVehicleWheelComponent* VehicleWheel = CreateDefaultSubobject<UVehicleWheelComponent>(TEXT("WheelComponent"));
}

void AVehiclePlayer::EnhancedDrift() {

	/*FVector DriftImpulse = FVector::CrossProduct(GetActorUpVector(), GetActorForwardVector()) * DriftForce * Value.GetMagnitude();
	GetMesh()->AddImpulseAtLocation(DriftImpulse, GetActorLocation());*/

	// GetVehicleMovementComponent()->SetHandbrakeInput(Value.Get<bool>());

	// GetVehicleMovementComponent()->

	UChaosWheeledVehicleMovementComponent* WheelComponent = Cast<UChaosWheeledVehicleMovementComponent>(GetVehicleMovementComponent());
	if (WheelComponent != nullptr)
	{
		WheelComponent->SetWheelFrictionMultiplier(0, 0.8);
		WheelComponent->SetWheelFrictionMultiplier(1, 0.8);
		WheelComponent->SetWheelFrictionMultiplier(2, 0.8);
		WheelComponent->SetWheelFrictionMultiplier(3, 0.8);
	}
}

void AVehiclePlayer::EnhancedDriftCompleted() {

	UChaosWheeledVehicleMovementComponent* WheelComponent = Cast<UChaosWheeledVehicleMovementComponent>(GetVehicleMovementComponent());
	if (WheelComponent != nullptr)
	{
		WheelComponent->SetWheelFrictionMultiplier(0, 3);
		WheelComponent->SetWheelFrictionMultiplier(1, 3);
		WheelComponent->SetWheelFrictionMultiplier(2, 3);
		WheelComponent->SetWheelFrictionMultiplier(3, 3);
	}
}

void AVehiclePlayer::EnhancedReset() {

}

void AVehiclePlayer::EnhancedBooster() {

	/*direction = GetActorForwardVector();
	direction.Normalize();
	OriginVelocity = GetMesh()->GetPhysicsLinearVelocity();
	FVector BoosterVelocity = direction * BoosterMultiplier * OriginVelocity;
	GetMesh()->SetPhysicsLinearVelocity(BoosterVelocity);
	OriginVelocity = GetVehicleMovementComponent()->GetForwardSpeed();*/

	RightThruster->SetAutoActivate(true);
	LeftThruster->SetAutoActivate(true);
	RightBoosterLight->ToggleVisibility(true);
	LeftBoosterLight->ToggleVisibility(true);
}



//void AVehiclePlayer::EnhancedBoosterCompleted() {
//
//	 RightThruster->SetAutoActivate(false);
//	 LeftThruster->SetAutoActivate(false);
//	RightBoosterLight->ToggleVisibility(false);
//	LeftBoosterLight->ToggleVisibility(false);
//}
