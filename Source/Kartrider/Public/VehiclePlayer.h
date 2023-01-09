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
#include "PhysicsEngine/PhysicsThrusterComponent.h"
#include "Components/PointLightComponent.h"

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

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Input Actions")
	UInputAction* BoosterAction;


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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Booster Settings")
	USceneCaptureComponent2D* SceneCaptureCameraComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Booster Settings")
	class UParticleSystemComponent* TurboEffectLeft;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Booster Settings")
	class UParticleSystemComponent* TurboEffectRight;

	UPROPERTY(EditDefaultsOnly, Category = "Booster Settings")
	bool IsBoost;

private:
	/*UPROPERTY(EditAnywhere, Category="Vehicle Player")
	USkeletalMeshComponent* VehicleMeshComponent;*/

	// Input Functions
	void EnhancedThrottle(const FInputActionValue& Value);

	void EnhancedSteering(const FInputActionValue& Value);

	void EnhancedBreak(const FInputActionValue& Value);

	void EnhancedDrift();
	void EnhancedDriftCompleted();

	void EnhancedReset();

	// void EnhancedBooster(const FInputActionValue& Value);
	// void EnhancedBoosterCompleted(const FInputActionValue& Value);


	FVector direction;
	float BoostValue = 100.0f;
	float BoostMultiplier = 0.04f;
	float DriftForce = 100.0f;
	float OriginVelocity;
};
