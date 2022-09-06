// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/JumpscareActors/SpawnEnemiesEvent.h"
#include "AI/Classic/AI_Basic.h"
#include "Characters/HGCharacter.h"

ASpawnEnemiesEvent::ASpawnEnemiesEvent()
{
	NumberOfAIToSpawn = 0;
	SpawnSpeed = 2.f;
	CurrentIndex = 0;
}

void ASpawnEnemiesEvent::BeginPlay()
{
	Super::BeginPlay();
}

void ASpawnEnemiesEvent::SpawnAI()
{
	FVector SpawnLocation = GetTransform().TransformPosition(TargetLocation);

	GetWorld()->SpawnActor<AAI_Basic>(AIClass, SpawnLocation, FRotator(0.f, 0.f, 0.f));

	if (CurrentIndex+1 == NumberOfAIToSpawn)
	{
		Destroy();
	}
}

void ASpawnEnemiesEvent::TriggerJumpscare(AActor* OverlappingActor)
{
	AHGCharacter* PlayerOverlap =  Cast<AHGCharacter>(OverlappingActor);
	if (!PlayerOverlap) return;

	for (int i = 0; i < NumberOfAIToSpawn; i++)
	{
		CurrentIndex = 0;

		FTimerHandle SpawnTimerHandle;
		GetWorld()->GetTimerManager().SetTimer(SpawnTimerHandle, this, &ASpawnEnemiesEvent::SpawnAI, SpawnSpeed, false);
	}
}
