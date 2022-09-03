// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Tasks/BTTask_PlayJumpscareAudio.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "AI/Classic/Basic_AIController.h"

UBTTask_PlayJumpscareAudio::UBTTask_PlayJumpscareAudio()
{
	NodeName = TEXT("Play Jumpscare Audio");
}

EBTNodeResult::Type UBTTask_PlayJumpscareAudio::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (!JumpscareSound) return EBTNodeResult::Failed;
	ABasic_AIController* ControllerAI = Cast<ABasic_AIController>(OwnerComp.GetAIOwner());
	if (!ControllerAI) return EBTNodeResult::Failed;
	
	if (ControllerAI->AlreadyPlayedJumpscareSound()) return EBTNodeResult::Succeeded;
	
	UGameplayStatics::PlaySound2D(GetWorld(), JumpscareSound);
	ControllerAI->SetAlreadyPlayedJumpscareSound(true);

	return EBTNodeResult::Succeeded;
}

FString UBTTask_PlayJumpscareAudio::GetStaticDescription() const
{
	return FString::Printf(TEXT("USoundCue : JumpscareSound SoundCue"));
}
