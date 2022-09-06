// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/JumpscareActors/JumpscareMaster.h"
#include "SpawnEnemiesEvent.generated.h"

class AAI_Basic;
/**
 * 
 */
UCLASS()
class HORRORGAME_API ASpawnEnemiesEvent : public AJumpscareMaster
{
	GENERATED_BODY()
	
public:
	ASpawnEnemiesEvent();

protected:

	virtual void BeginPlay() override;

private:

	/* Class for the AI to be spawned */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AAI_Basic> AIClass;

	/* Locations where to spawn actors */
	UPROPERTY(EditAnywhere, Meta = (MakeEditWidget = true))
	FVector TargetLocation;
	
	UPROPERTY(EditAnywhere, Meta = (MakeEditWidget = true))
	int NumberOfAIToSpawn;

	/* The amount of time (in seconds) between the spawning of each AI */
	UPROPERTY(EditAnywhere, Meta = (MakeEditWidget = true))
	float SpawnSpeed;

	void SpawnAI();

	int CurrentIndex;

protected:

	/* Instead of triggering a jumpscare, it will spawn an AI at a specific location */
	virtual void TriggerJumpscare(AActor* OverlappingActor) override;
};
