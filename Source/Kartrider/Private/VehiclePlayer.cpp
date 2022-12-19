// Fill out your copyright notice in the Description page of Project Settings.


#include "VehiclePlayer.h"


AVehiclePlayer::AVehiclePlayer()
{
	// 자동차를 위한 스켈레탈 메시 컴포넌트 생성 후 루트 컴포넌트로 설정
	VehicleMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("VehicleMeshComponent"));
	RootComponent = VehicleMeshComponent;
	
	// 스프링 암 컴포넌트 생성 후 루트 컴포넌트에 붙인다.
	VehicleSpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("VehicleSpringArmComponent"));
	VehicleSpringArmComponent->SetupAttachment(VehicleMeshComponent);

	// 카메라 컴포넌트 생성 후 스프링 암 컴포넌트에 붙인다;
	VehicleCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("VehicleCameraComponent"));
	VehicleCameraComponent->SetupAttachment(VehicleSpringArmComponent);

}

void AVehiclePlayer::EnhancedThrottle(const FInputActionValue Value) {
	if (Value.GetMagnitude() != 0.0f)
	{
		GetVehicleMovement()->SetThrottleInput(Value.GetMagnitude());
	}
}