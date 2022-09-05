// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/JumpscareActors/JumpscareMaster.h"
#include "EnemyRevealCutsceneEvent.generated.h"


class ULevelSequence;
class ULevelSequencePlayer;
class UMainHUD;
class AAI_Basic;
/**
 * 
 */
UCLASS()
class HORRORGAME_API AEnemyRevealCutsceneEvent : public AJumpscareMaster
{
	GENERATED_BODY()
	

private:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	ULevelSequence* Cutscene;

	ULevelSequencePlayer* LevelSequenceP;

	UMainHUD* PlayerCharacterHUD;

	float CutsceneDuration;

	/* Deletes skeletalmesh actor after cutscene, before spawning real AI */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	AActor* ActorToDeleteAfterCutscene;

	/* Class for the AI to be spawned */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AAI_Basic> AIClass;
protected:

	/* Instead of triggering a jumpscare, it will trigger a cutscene */
	virtual void TriggerJumpscare(AActor* OverlappingActor) override;

	/* Plays the ULevelSequence* Cutscene when player overlaps the collision box created in master class */
	virtual void PlayCutscene();

	/* Calls a timer that will call the FinishDisplayHUD to unbind delegate and to display HUD */
	UFUNCTION()
	void StartDisplayingHUD();
	void FinishDisplayHUD();

public:
};
