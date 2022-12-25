// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WheeledVehiclePawn.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "Components/SceneCaptureComponent2D.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "ChaosVehicleMovementComponent.h"
#include "EnhancedInputComponent.h"

#include "VehiclePlayer.generated.h"

/**
 * 
 */
UCLASS()
class KARTRIDER_API AVehiclePlayer : public AWheeledVehiclePawn
{
	GENERATED_BODY() // ������ �ڽ��� ���� ������ �ؼ� ������.
	
public:
	AVehiclePlayer();

	void BeginPlay();

	void Tick(float DeltaTime);

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

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

	/*UPROPERTY(EditAnywhere, Category="Input")
	class UInputMappingContext* InputMapping;*/

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vehicle Player")
	USpringArmComponent* VehicleSpringArmComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vehicle Player")
	UCameraComponent* VehicleCameraComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vehicle Player")
	USpringArmComponent* SceneCaptureSpringArmComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vehicle Player")
	USceneCaptureComponent2D* SceneCaptureCameraComponent;

private:
	/*UPROPERTY(EditAnywhere, Category="Vehicle Player")
	USkeletalMeshComponent* VehicleMeshComponent;*/

	// Input Functions
	void EnhancedThrottle(const FInputActionValue& Value);

	void EnhancedSteering(const FInputActionValue& Value);

	void EnhancedBreak(const FInputActionValue& Value);

	void EnhancedDrift(const FInputActionValue& Value);

	void EnhancedReset(const FInputActionValue& Value);
};
