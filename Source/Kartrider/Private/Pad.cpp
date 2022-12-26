// Fill out your copyright notice in the Description page of Project Settings.


#include "Pad.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "LaunchPad.generated.h"

// Sets default values
APad::APad()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collision"));
	RootComponent = BoxCollision;


	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
	StaticMesh->SetupAttachment(RootComponent);

}

void ALaunchPad::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Character Launched!"));
	Asgd240_1115350_core5Character::LaunchCharacter(FVector(0, 0, velocity), false, true);

}


// Called when the game starts or when spawned
void APad::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APad::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

