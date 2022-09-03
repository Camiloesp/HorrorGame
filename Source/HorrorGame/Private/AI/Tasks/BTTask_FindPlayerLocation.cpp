// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Tasks/BTTask_FindPlayerLocation.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "Kismet/GameplayStatics.h"

#include "AI/Classic/AI_Basic.h"
#include "AI/Classic/Basic_AIController.h"
#include "Characters/HGCharacter.h"

UBTTask_FindPlayerLocation::UBTTask_FindPlayerLocation()
{
	NodeName = TEXT("Find and Chase Player");

	PlayerLocationKey = FVector(0.f, 0.f, 0.f);
}

EBTNodeResult::Type UBTTask_FindPlayerLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAI_Basic* AIOwner = Cast<AAI_Basic>(OwnerComp.GetAIOwner()->GetPawn());
	if (!AIOwner) return EBTNodeResult::Failed;
	ABasic_AIController* ControllerAI = Cast<ABasic_AIController>(AIOwner->GetController());
	if (!ControllerAI) return EBTNodeResult::Failed;

	//Get player location
	AHGCharacter* PlayerChar = ControllerAI->GetPlayerTarget();
	if (!PlayerChar) return EBTNodeResult::Failed;
	PlayerLocationKey = PlayerChar->GetActorLocation();

	// Set reference to the target location BB value
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	BlackboardComp->SetValueAsVector(FName("PlayerLocation"), PlayerLocationKey);

	// Move AI to PlayerLocationKey
	ControllerAI->MoveToLocation(PlayerLocationKey);

	return EBTNodeResult::Succeeded;
}

FString UBTTask_FindPlayerLocation::GetStaticDescription() const
{
	return FString::Printf(TEXT("Vector : %s"), *BlackboardKey.SelectedKeyName.ToString());
}