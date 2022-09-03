// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Tasks/BTTask_LastPlayerLocation.h"
#include "AI/Classic/Basic_AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "Characters/HGCharacter.h"

UBTTask_LastPlayerLocation::UBTTask_LastPlayerLocation()
{
	NodeName = TEXT("Find and Chase Player");

	TargetLocationKey = FVector(0.f, 0.f, 0.f);
}

EBTNodeResult::Type UBTTask_LastPlayerLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ABasic_AIController* ControllerAI = Cast<ABasic_AIController>(OwnerComp.GetAIOwner());
	if (!ControllerAI) return EBTNodeResult::Failed;
	AHGCharacter* PlayerTarget = ControllerAI->GetPlayerTarget();
	if (!PlayerTarget) return EBTNodeResult::Failed;

	// Set TargetLocationKey
	TargetLocationKey = PlayerTarget->GetActorLocation();

	// Set reference to the target location BB value
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	BlackboardComp->SetValueAsVector(FName("TargetLocation"), TargetLocationKey);


	return EBTNodeResult::Succeeded;
}

FString UBTTask_LastPlayerLocation::GetStaticDescription() const
{
	return FString::Printf(TEXT("Vector : %s"), *BlackboardKey.SelectedKeyName.ToString());
}
