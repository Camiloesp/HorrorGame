// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionComponent.h" // Needs to be included here, else it won't compile
#include "../../Interfaces/PlayerHiding.h"
#include "Basic_AIController.generated.h"

class UAIPerceptionComponent;
class UAISenseConfig_Sight;
struct FAIStimulus;
class AHGCharacter;
class AHideActor;
/**
 * 
 */
UCLASS()
class HORRORGAME_API ABasic_AIController : public AAIController, public IPlayerHiding
{
	GENERATED_BODY()
	
public:

	ABasic_AIController();

protected:

	virtual void BeginPlay() override;

private:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	UBehaviorTree* BT_BasicAI;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	UAIPerceptionComponent* Perception;

	UAISenseConfig_Sight* SightSense;

	bool bCanSeePlayer;
	bool bAlreadyPlayedJumpscareSound;

	AHGCharacter* PlayerTarget;

	AHideActor* PlayerTargetHidingSpot;
protected:
public:

	virtual void DidEnemySee(AHideActor* HideActor) override;
	virtual void LeftHidingSpot() override;

	void PullOutOfHiding();

	UFUNCTION()
	void OnPerception(AActor* Actor, FAIStimulus Stimulos);


	FORCEINLINE AHGCharacter* GetPlayerTarget() const { return PlayerTarget; }
	FORCEINLINE bool AlreadyPlayedJumpscareSound() const { return bAlreadyPlayedJumpscareSound; }

	FORCEINLINE void SetAlreadyPlayedJumpscareSound(bool NewBool) { bAlreadyPlayedJumpscareSound = NewBool; }
};
