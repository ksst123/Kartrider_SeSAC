// Fill out your copyright notice in the Description page of Project Settings.


#include "VehiclePlayer.h"
#include "EnhancedInputSubsystems.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/LocalPlayer.h"
#include "GameFramework/PlayerController.h"
#include "ChaosWheeledVehicleMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"






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

	// 부스터 효과를 위한 파티클 생성 후 루트 컴포넌트에 붙인다.
	TurboEffectLeft = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Left Booster Effect"));
	TurboEffectRight = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Right Booster Effect"));
	TurboEffectLeft->SetupAttachment(RootComponent);
	TurboEffectRight->SetupAttachment(RootComponent);



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

	MyDeltaTime = DeltaTime;
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
	Input->BindAction(BoosterAction, ETriggerEvent::Completed, this, &AVehiclePlayer::EnhancedBoosterCompleted);

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

	FVector DriftImpulse = FVector::CrossProduct(FVector::UpVector, GetActorForwardVector()) * DriftForce; // * Value.GetMagnitude();
	GetMesh()->AddImpulseAtLocation(DriftImpulse, GetActorLocation());

	UChaosWheeledVehicleMovementComponent* WheelComponent = Cast<UChaosWheeledVehicleMovementComponent>(GetVehicleMovementComponent());
	if (WheelComponent != nullptr)
	{
		/*FVector DriftImpulse = FVector::CrossProduct(GetActorUpVector(), GetActorForwardVector()) * DriftForce * Value.GetMagnitude();
		GetMesh()->AddImpulseAtLocation(DriftImpulse, GetActorLocation());*/

		WheelComponent->SetWheelFrictionMultiplier(0, 0.8);
		WheelComponent->SetWheelFrictionMultiplier(1, 0.8);
		WheelComponent->SetWheelFrictionMultiplier(2, 1.5);
		WheelComponent->SetWheelFrictionMultiplier(3, 1.5);

		/*WheelComponent->SetWheelSlipGraphMultiplier(0, 10);
		WheelComponent->SetWheelSlipGraphMultiplier(1, 10);
		WheelComponent->SetWheelSlipGraphMultiplier(2, 12);
		WheelComponent->SetWheelSlipGraphMultiplier(3, 12);*/

		/*FVector DriftValue = FVector::CrossProduct(GetActorUpVector(), GetActorForwardVector()) * 10000;
		GetMesh()->AddImpulse(DriftValue);*/
	}
}

void AVehiclePlayer::EnhancedDriftCompleted() {

	GetMesh()->AddImpulseAtLocation(FVector(0.0f), GetActorLocation());

	UChaosWheeledVehicleMovementComponent* WheelComponent = Cast<UChaosWheeledVehicleMovementComponent>(GetVehicleMovementComponent());
	if (WheelComponent != nullptr)
	{
		WheelComponent->SetWheelFrictionMultiplier(0, 1);
		WheelComponent->SetWheelFrictionMultiplier(1, 1);
		WheelComponent->SetWheelFrictionMultiplier(2, 2);
		WheelComponent->SetWheelFrictionMultiplier(3, 2);

		/*WheelComponent->SetWheelSlipGraphMultiplier(0, 1);
		WheelComponent->SetWheelSlipGraphMultiplier(1, 1);
		WheelComponent->SetWheelSlipGraphMultiplier(2, 2);
		WheelComponent->SetWheelSlipGraphMultiplier(3, 2);*/
	}
}

void AVehiclePlayer::EnhancedReset() {

}

void AVehiclePlayer::EnhancedBooster(const FInputActionValue& Value) {

	//direction = GetActorForwardVector();
	//direction.Normalize();
	//// OriginVelocity = GetMesh()->GetPhysicsLinearVelocity();
	//OriginVelocity = GetVehicleMovementComponent()->GetForwardSpeed();
	///*UE_LOG(LogTemp, Warning, TEXT("%s"), *OriginVelocity.ToString());
	//UE_LOG(LogTemp, Warning, TEXT("%f"), GetVehicleMovementComponent()->GetForwardSpeed());*/
	//FVector BoosterVelocity = direction * BoosterMultiplier * OriginVelocity;
	//GetMesh()->SetPhysicsLinearVelocity(BoosterVelocity, true);
	//// OriginVelocity = GetVehicleMovementComponent()->GetForwardSpeed();

	//RightThruster->SetAutoActivate(true);
	//LeftThruster->SetAutoActivate(true);
	//RightBoosterLight->ToggleVisibility(true);
	//LeftBoosterLight->ToggleVisibility(true);

	// Turbo 파티클 활성화
	IsBoost = true;
	TurboEffectLeft->ToggleVisibility(true);
	TurboEffectRight->ToggleVisibility(true);
	
	float BoostDuration = Value.GetMagnitude() * 5.0f;

	while (BoostValue >= 0.0f)
	{
		// Booster Physics 적용
		direction = GetActorForwardVector();
		direction.Normalize();
		float CurrentSpeed = GetVehicleMovementComponent()->GetForwardSpeed();
		FVector BoostVelocity = direction * CurrentSpeed * BoostMultiplier;
		GetMesh()->SetPhysicsLinearVelocity(BoostVelocity * 0.3, true);

		BoostValue -= BoostDuration * MyDeltaTime;
		UE_LOG(LogTemp, Warning, TEXT("%f"), BoostValue);
	}
}



void AVehiclePlayer::EnhancedBoosterCompleted(const FInputActionValue& Value) {

	// GetMesh()->SetPhysicsLinearVelocity(OriginVelocity);

	/*RightThruster->SetAutoActivate(false);
	LeftThruster->SetAutoActivate(false);
	RightBoosterLight->ToggleVisibility(false);
	LeftBoosterLight->ToggleVisibility(false);*/

	// Turbo 파티클 비활성화
	IsBoost = false;
	TurboEffectLeft->ToggleVisibility(false);
	TurboEffectRight->ToggleVisibility(false);
}
