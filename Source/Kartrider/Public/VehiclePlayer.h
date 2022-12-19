// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WheeledVehiclePawn.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "ChaosVehicleMovementComponent.h"

#include "VehiclePlayer.generated.h"

/**
 * 
 */
UCLASS()
class KARTRIDER_API AVehiclePlayer : public AWheeledVehiclePawn
{
	GENERATED_BODY() // 실제로 자신을 씬에 렌더링 해서 만들어낸다.
	
public:
	AVehiclePlayer();

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	// Input Actions
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Input Actions")
	UInputAction* ThrottleAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category ="Input Actions")
	UInputAction* SteeringAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Input Actions")
	UInputAction* BreakAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Input Actions")
	UInputAction* DriftAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Input Actions")
	UInputAction* ResetAction;


	// Input Mappings
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Input Mappings")
	UInputMappingContext* BaseMappingContext;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Input Mappings")
	int32 BaseMappingPrioirty = 0;

private:
	UPROPERTY()
	USkeletalMeshComponent* VehicleMeshComponent;

	UPROPERTY()
	USpringArmComponent* VehicleSpringArmComponent;

	UPROPERTY()
	UCameraComponent* VehicleCameraComponent;

	// Input Functions
	void EnhancedThrottle(const FInputActionValue& Value);

	void EnhancedSteering(const FInputActionValue& Value);

	void EnhancedBreak(const FInputActionValue& Value);

	void EnhancedDrift(const FInputActionValue& Value);
};
