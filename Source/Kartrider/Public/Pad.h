// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h" //헤더파일에 선언한다고 한다. 


#include "Pad.generated.h" //주의사항: generated.h 항상 맨아래에 있어야한다. 언리얼엔진에서 이거 생기면 밑으로 더이상 파일을 읽지않음. 


UCLASS()


class  KARTRIDER_API APad : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	Pad();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void OnBoxBeginOverlap(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UPROPERTY(VisibleAnywhere)
		class UBoxComponent* Collide;

	UFUNCTION()
		void OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(EditAnywhere)
		float velocity;

	UFUNCTION()
		void LaunchCharacter();

};
