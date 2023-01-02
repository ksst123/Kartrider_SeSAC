// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "KartriderUI.generated.h"

/**
 * 
 */
UCLASS()
class KARTRIDER_API UKartriderUI : public UUserWidget
{
	GENERATED_BODY()
	

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kartrider UI|Speed")
	class UTextBlock* CarSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kartrider UI|Speed")
	class UTextBlock* Gear;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kartrider UI|Speedometer")
	class UImage* Needle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kartrider UI|Speedometer")
	class UImage* SpeedometerImage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kartrider UI|Minimap")
	class UImage* Minimap;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kartrider UI|Racing")
	class UTextBlock* CurrentLapText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kartrider UI|Racing")
	class UTextBlock* CurrentTimeText;

	/*UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kartrider UI|Racing")
	int32 MaxLap;*/

	/*UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kartrider UI|Racing")
	float FinishTime;*/


	void PrintCurrentTime();
	void PrintCurrentLap();
	void PrintMinimap();
	void PrintSpeedometer();
	void PrintSpeed();
};
