// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Tasks/BTTask_KillPlayer.h"
#include "AI/Classic/Basic_AIController.h"

#include "Characters/HGCharacter.h"

UBTTask_KillPlayer::UBTTask_KillPlayer()
{
    NodeName = TEXT("Kill player");
}

EBTNodeResult::Type UBTTask_KillPlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    ABasic_AIController* ControllerAI = Cast<ABasic_AIController>(OwnerComp.GetAIOwner());
    if (!ControllerAI) return EBTNodeResult::Failed;

    AHGCharacter* PlayerTarget = ControllerAI->GetPlayerTarget();
    if (!PlayerTarget) return EBTNodeResult::Failed;

    PlayerTarget->CaughtByEnemy(ControllerAI->GetPawn());

    return EBTNodeResult::Succeeded;
}

FString UBTTask_KillPlayer::GetStaticDescription() const
{
    return FString::Printf(TEXT("Kill player"));
}
