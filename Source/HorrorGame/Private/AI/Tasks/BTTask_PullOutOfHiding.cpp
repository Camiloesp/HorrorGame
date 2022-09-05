// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Tasks/BTTask_PullOutOfHiding.h"
#include "AI/Classic/Basic_AIController.h"

UBTTask_PullOutOfHiding::UBTTask_PullOutOfHiding()
{
	NodeName = TEXT("Pull player target out of hiding");
}

EBTNodeResult::Type UBTTask_PullOutOfHiding::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ABasic_AIController* ControllerAI = Cast<ABasic_AIController>(OwnerComp.GetAIOwner());
	if (!ControllerAI) return EBTNodeResult::Failed;

	ControllerAI->PullOutOfHiding();

	return EBTNodeResult::Succeeded;
}

FString UBTTask_PullOutOfHiding::GetStaticDescription() const
{
	return FString::Printf(TEXT("%s"), *BlackboardKey.SelectedKeyName.ToString());
}
