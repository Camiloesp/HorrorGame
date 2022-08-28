// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/JumpscareActors/EnemyRevealCutsceneEvent.h"
#include "Characters/HGCharacter.h"
#include "Widgets/MainHUD.h"

#include "LevelSequence.h"
#include "LevelSequencePlayer.h"
#include "MovieSceneSequencePlayer.h"


void AEnemyRevealCutsceneEvent::TriggerJumpscare(AActor* OverlappingActor)
{
	// Hide HUD
	AHGCharacter* PlayerCharacter = Cast<AHGCharacter>(OverlappingActor);
	if (!PlayerCharacter) return;
	PlayerCharacterHUD = PlayerCharacter->GetMainHUDRef();
	if (!PlayerCharacterHUD) return;
	PlayerCharacterHUD->SetVisibility(ESlateVisibility::Hidden);

	PlayCutscene();
}

void AEnemyRevealCutsceneEvent::PlayCutscene()
{
	if (!Cutscene) return;

	// Cutscene settings
	FMovieSceneSequencePlaybackSettings CutsceneSettings;
	CutsceneSettings.bHidePlayer = true;
	CutsceneSettings.bHideHud = true;
	CutsceneSettings.bDisableMovementInput = true;
	CutsceneSettings.bDisableLookAtInput = true;
	
	// Play cutscene
	ALevelSequenceActor* OutLevelSequenceActor;
	LevelSequenceP = ULevelSequencePlayer::CreateLevelSequencePlayer(GetWorld(), Cutscene, CutsceneSettings, OutLevelSequenceActor);
	if (LevelSequenceP)
	{
		LevelSequenceP->OnFinished.AddDynamic(this, &AEnemyRevealCutsceneEvent::StartDisplayingHUD);
		LevelSequenceP->Play();
	}
	
}

void AEnemyRevealCutsceneEvent::StartDisplayingHUD()
{
	if (!LevelSequenceP || !Cutscene) return;

	//remove delegate binding
	LevelSequenceP->OnFinished.RemoveDynamic(this, &AEnemyRevealCutsceneEvent::StartDisplayingHUD);

	// small delay to display HUD after LevelSequence is done
	FTimerHandle DisplayHUDTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(DisplayHUDTimerHandle, this, &AEnemyRevealCutsceneEvent::FinishDisplayHUD, 0.5f, false);
}

void AEnemyRevealCutsceneEvent::FinishDisplayHUD()
{
	if (!LevelSequenceP || !PlayerCharacterHUD) return;
	
	PlayerCharacterHUD->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	Destroy();
}
