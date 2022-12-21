// Fill out your copyright notice in the Description page of Project Settings.


#include "VehiclePlayer.h"
#include "EnhancedInputSubsystems.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/LocalPlayer.h"
#include "GameFramework/PlayerController.h"


AVehiclePlayer::AVehiclePlayer()
{
	PrimaryActorTick.bCanEverTick = true;

	// �ڵ����� ���� ���̷�Ż �޽� ������Ʈ ���� �� ��Ʈ ������Ʈ�� ����
	VehicleMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("VehicleMeshComponent"));
	RootComponent = VehicleMeshComponent;
	
	// ������ �� ������Ʈ ���� �� ��Ʈ ������Ʈ�� ���δ�.
	VehicleSpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("VehicleSpringArmComponent"));
	VehicleSpringArmComponent->SetupAttachment(RootComponent);

	// ī�޶� ������Ʈ ���� �� ������ �� ������Ʈ�� ���δ�;
	VehicleCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("VehicleCameraComponent"));
	VehicleCameraComponent->SetupAttachment(VehicleSpringArmComponent);

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
	subsys->AddMappingContext(InputMapping, 0);

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
	Input->BindAction(DriftAction, ETriggerEvent::Started, this, &AVehiclePlayer::EnhancedBreak);
	Input->BindAction(DriftAction, ETriggerEvent::Completed, this, &AVehiclePlayer::EnhancedBreak);

	// ���� �׼� ���ε�
	Input->BindAction(ResetAction, ETriggerEvent::Triggered, this, &AVehiclePlayer::EnhancedReset);
	

}

void AVehiclePlayer::EnhancedThrottle(const FInputActionValue& Value) {
	if (Value.GetMagnitude() != 0.0f)
	{
		// GetVehicleMovementComponent()->SetThrottleInput(Value.GetMagnitude());
		GetVehicleMovementComponent()->SetThrottleInput(Value.GetMagnitude());
	}
}

void AVehiclePlayer::EnhancedSteering(const FInputActionValue& Value) {
}

void AVehiclePlayer::EnhancedBreak(const FInputActionValue& Value) {
	GetVehicleMovementComponent()->SetBrakeInput(Value.GetMagnitude());
}

void AVehiclePlayer::EnhancedDrift(const FInputActionValue& Value) {

}

void AVehiclePlayer::EnhancedReset(const FInputActionValue& Value) {

}
