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

	// 자동차를 위한 스켈레탈 메시 컴포넌트 생성 후 루트 컴포넌트로 설정
	/*VehicleMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("VehicleMeshComponent"));
	RootComponent = VehicleMeshComponent;*/
	
	// 스프링 암 컴포넌트 생성 후 루트 컴포넌트에 붙인다.
	VehicleSpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("VehicleSpringArmComponent"));
	VehicleSpringArmComponent->SetupAttachment(RootComponent);

	// 카메라 컴포넌트 생성 후 스프링 암 컴포넌트에 붙인다.
	VehicleCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("VehicleCameraComponent"));
	VehicleCameraComponent->SetupAttachment(VehicleSpringArmComponent);

	// 캡쳐 카메라를 위한 스프링 암 컴포넌트 생성 후 루트 컴포넌트에 붙인다.
	SceneCaptureSpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SceneCaptureSpringArmComponent"));
	SceneCaptureSpringArmComponent->SetupAttachment(RootComponent);

	SceneCaptureCameraComponent = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("SceneCaptureCameraComponent"));
	SceneCaptureCameraComponent->SetupAttachment(SceneCaptureSpringArmComponent);

	// 부스터를 위한 오른쪽 Thruster 생성 후 루트 컴포넌트에 붙인다.
	RightThruster = CreateDefaultSubobject<UPhysicsThrusterComponent>(TEXT("RightBoosterThruster"));
	RightThruster->SetupAttachment(RootComponent);
	RightThruster->SetAutoActivate(false);

	// 부스터를 위한 왼쪽 Thruster 생성 후 루트 컴포넌트에 붙인다.
	LeftThruster = CreateDefaultSubobject<UPhysicsThrusterComponent>(TEXT("LeftBoosterThruster"));
	LeftThruster->SetupAttachment(RootComponent);
	LeftThruster->SetAutoActivate(false);

	// 부스터 효과를 위한 오른쪽 Point Light 생성 후 오른쪽 Thruster에 붙인다.
	RightBoosterLight = CreateDefaultSubobject<UPointLightComponent>(TEXT("RightBoosterLight"));
	RightBoosterLight->SetupAttachment(RightThruster);
	RightBoosterLight->ToggleVisibility(false);
	RightBoosterLight->SetCastShadows(false);

	// 부스터 효과를 위한 왼쪽 Point Light 생성 후 왼쪽 Thruster에 붙인다.
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

	// 쓰로틀 액션 바인딩
	Input->BindAction(ThrottleAction, ETriggerEvent::Triggered, this, &AVehiclePlayer::EnhancedThrottle);
	Input->BindAction(ThrottleAction, ETriggerEvent::Completed, this, &AVehiclePlayer::EnhancedThrottle);

	// 스티어링 액션 바인딩
	Input->BindAction(SteeringAction, ETriggerEvent::Triggered, this, &AVehiclePlayer::EnhancedSteering);
	Input->BindAction(SteeringAction, ETriggerEvent::Completed, this, &AVehiclePlayer::EnhancedSteering);

	// 브레이크 액션 바인딩
	Input->BindAction(BreakAction, ETriggerEvent::Triggered, this, &AVehiclePlayer::EnhancedBreak);
	Input->BindAction(BreakAction, ETriggerEvent::Started, this, &AVehiclePlayer::EnhancedBreak);
	Input->BindAction(BreakAction, ETriggerEvent::Completed, this, &AVehiclePlayer::EnhancedBreak);

	// 드리프트 액션 바인딩
	Input->BindAction(DriftAction, ETriggerEvent::Triggered, this, &AVehiclePlayer::EnhancedDrift);
	Input->BindAction(DriftAction, ETriggerEvent::Started, this, &AVehiclePlayer::EnhancedDrift);
	Input->BindAction(DriftAction, ETriggerEvent::Ongoing, this, &AVehiclePlayer::EnhancedDrift);
	Input->BindAction(DriftAction, ETriggerEvent::Completed, this, &AVehiclePlayer::EnhancedDriftCompleted);

	// 리셋 액션 바인딩
	Input->BindAction(ResetAction, ETriggerEvent::Triggered, this, &AVehiclePlayer::EnhancedReset);
	
	// 부스터 액션 바인딩
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
