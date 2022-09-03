// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Tasks/BTTask_SearchForPlayer.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NavigationSystem.h"

UBTTask_SearchForPlayer::UBTTask_SearchForPlayer()
{
	NodeName = TEXT("Search for player");

	TargetLocationKey = FVector(0.f, 0.f, 0.f);
	SearchRadius = 250.f;
}

EBTNodeResult::Type UBTTask_SearchForPlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();

	// Investigate random location from last location where AI saw the player
	TargetLocationKey = BlackboardComp->GetValueAsVector(FName("TargetLocation"));
	FVector OutRandomLocation;
	UNavigationSystemV1::K2_GetRandomLocationInNavigableRadius(GetWorld(), TargetLocationKey, OutRandomLocation, SearchRadius);

	BlackboardComp->SetValueAsVector(FName("TargetLocation"), OutRandomLocation);

	return EBTNodeResult::Succeeded;
}

FString UBTTask_SearchForPlayer::GetStaticDescription() const
{
	return FString::Printf(TEXT("Vector : %s"), *BlackboardKey.SelectedKeyName.ToString());
}
