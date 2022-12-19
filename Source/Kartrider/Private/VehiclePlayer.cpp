// Fill out your copyright notice in the Description page of Project Settings.


#include "VehiclePlayer.h"


AVehiclePlayer::AVehiclePlayer()
{
	// �ڵ����� ���� ���̷�Ż �޽� ������Ʈ ���� �� ��Ʈ ������Ʈ�� ����
	VehicleMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("VehicleMeshComponent"));
	RootComponent = VehicleMeshComponent;
	
	// ������ �� ������Ʈ ���� �� ��Ʈ ������Ʈ�� ���δ�.
	VehicleSpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("VehicleSpringArmComponent"));
	VehicleSpringArmComponent->SetupAttachment(VehicleMeshComponent);

	// ī�޶� ������Ʈ ���� �� ������ �� ������Ʈ�� ���δ�;
	VehicleCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("VehicleCameraComponent"));
	VehicleCameraComponent->SetupAttachment(VehicleSpringArmComponent);

}

void AVehiclePlayer::EnhancedThrottle(const FInputActionValue Value) {
	if (Value.GetMagnitude() != 0.0f)
	{
		GetVehicleMovement()->SetThrottleInput(Value.GetMagnitude());
	}
}