// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/JumpscareActors/EnemyRevealCutsceneEvent.h"
#include "Characters/HGCharacter.h"
#include "Widgets/MainHUD.h"

#include "LevelSequence.h"
#include "LevelSequencePlayer.h"
#include "MovieSceneSequencePlayer.h"

#include "AI/Classic/AI_Basic.h"


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
		CutsceneDuration = LevelSequenceP->GetDuration().AsSeconds();
		LevelSequenceP->OnFinished.AddDynamic(this, &AEnemyRevealCutsceneEvent::StartDisplayingHUD);
		LevelSequenceP->Play();
	}
	
}

void AEnemyRevealCutsceneEvent::StartDisplayingHUD()
{
	if (!LevelSequenceP || !Cutscene) return;

	//remove delegate binding
	LevelSequenceP->OnFinished.RemoveDynamic(this, &AEnemyRevealCutsceneEvent::StartDisplayingHUD);


	// Destroy placeholder actor before spawning AI
	if (ActorToDeleteAfterCutscene)
	{
		FVector SpawnLocation = ActorToDeleteAfterCutscene->GetActorLocation();
		SpawnLocation.Z += 88.5;
		FRotator SpawnRotation = FRotator(0.f, 0.f, 0.f);

		ActorToDeleteAfterCutscene->Destroy();
		//Spawn AI
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		AAI_Basic* SpawnedAI = GetWorld()->SpawnActor<AAI_Basic>(AIClass, SpawnLocation, SpawnRotation, SpawnParams);
		if (SpawnedAI)
		{
			UE_LOG(LogTemp, Warning, TEXT("SPAWNED AI"));
		}
	}

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
