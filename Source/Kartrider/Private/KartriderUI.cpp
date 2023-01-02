// Fill out your copyright notice in the Description page of Project Settings.


#include "KartriderUI.h"
#include "KartriderGameModeBase.h"
#include "kismet/GameplayStatics.h"

void UKartriderUI::PrintCurrentTime() {
	
	AKartriderGameModeBase* KartriderGM = Cast<AKartriderGameModeBase>(UGameplayStatics::GetGameMode(this));

	if (KartriderGM != nullptr)
	{

	}
}

void UKartriderUI::PrintCurrentLap() {

}

void UKartriderUI::PrintMinimap() {

}

void UKartriderUI::PrintSpeedometer() {

}

void UKartriderUI::PrintSpeed() {

}
