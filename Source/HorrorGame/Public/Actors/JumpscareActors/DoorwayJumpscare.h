// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/JumpscareActors/JumpscareMaster.h"
#include "../../Interfaces/ObjectiveCall.h"
#include "DoorwayJumpscare.generated.h"

class UArrowComponent;
//class AnimationAsset;

/**
 * 
 */
UCLASS()
class HORRORGAME_API ADoorwayJumpscare : public AJumpscareMaster, public IObjectiveCall
{
	GENERATED_BODY()

public:

	ADoorwayJumpscare();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void OnConstruction(const FTransform& Transform) override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void TriggerJumpscare(AActor* OverlappingActor) override;

private:

	/* Mesh for the monster that will scare the player */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Jumpscare")
	USkeletalMeshComponent* Mesh;

	/* Point where the mesh stops moving */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UArrowComponent* EndPoint;

	bool bCalledJumpscare;

	/* Speed for the animation to play when scaring */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Jumpscare")
	float AnimationInterpSpeed;

	/* Time in seconds to destroy this event */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Jumpscare")
	float SecondsToDestroyThisEvent;

	///* Animation our mesh will play when scaring the player */
	//UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Jumpscare")
	//UAnimationAsset* AnimationToPlay;

protected:

	/* Destroy this jumpscare event after being triggered */
	UFUNCTION()
	void DestroyJumpscare();
	

	virtual void CompleteObjectiveCall(FName ObjectiveID, bool SetNextObjectiveImmediately);
	virtual void SetNextObjective(FName ObjectiveID);
};
