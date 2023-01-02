// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "KartriderGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class KARTRIDER_API AKartriderGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, Category="UI Settings")
	TSubclassOf<class UKartriderUI> RacingUI;

	UFUNCTION(BlueprintPure)
	FORCEINLINE int32 GetCurrentLap() { return CurrentLap; }

private:
	int32 CurrentLap = 1;
	int32 MaxLap = 3;
	float FinishTime;
};
