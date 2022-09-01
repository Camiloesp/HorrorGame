// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_FindRandomRoom.h"
#include "AI/Classic/AI_Basic.h"
#include "AI/Classic/Basic_AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "AI/AI_Helpers/RoomTarget.h"
#include "NavigationSystem.h"

UBTTask_FindRandomRoom::UBTTask_FindRandomRoom()
{
	NodeName = TEXT("Find Random Room");

	TargetLocationKey = FVector(0.f, 0.f, 0.f);
}

EBTNodeResult::Type UBTTask_FindRandomRoom::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	// Get all actor targets in the level, and pick one at random and sets the TargetLocationKey
	TArray<AActor*> RoomTargetActors;
	UGameplayStatics::GetAllActorsOfClass(OwnerComp.GetAIOwner()->GetWorld(), ARoomTarget::StaticClass(), RoomTargetActors); // AIOwner->RoomTargetClass
	int RandomNumber = UKismetMathLibrary::RandomIntegerInRange(0, RoomTargetActors.Num()-1);
	ARoomTarget* RandomTarget = Cast<ARoomTarget>(RoomTargetActors[RandomNumber]);
	TargetLocationKey = RandomTarget->GetActorLocation();

	// Randomly move within the RoomTarget radius
	float Radius = RandomTarget->GetNewSphereRadius();
	FVector OutRandomLocation;
	UNavigationSystemV1::K2_GetRandomLocationInNavigableRadius(GetWorld(), TargetLocationKey, OutRandomLocation, Radius);
	TargetLocationKey = OutRandomLocation;


	// Set reference to the target location BB value
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	BlackboardComp->SetValueAsVector(FName("TargetLocation"), TargetLocationKey);


	return EBTNodeResult::Succeeded;
}

FString UBTTask_FindRandomRoom::GetStaticDescription() const
{
	return FString::Printf(TEXT("Vector : %s"), *BlackboardKey.SelectedKeyName.ToString());
}
