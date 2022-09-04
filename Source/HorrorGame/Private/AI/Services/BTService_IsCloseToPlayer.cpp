// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Services/BTService_IsCloseToPlayer.h"
#include "AI/Classic/Basic_AIController.h"
#include "AI/Classic/AI_Basic.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "Kismet/KismetMathLibrary.h"
#include "Characters/HGCharacter.h"

UBTService_IsCloseToPlayer::UBTService_IsCloseToPlayer()
{
	NodeName = TEXT("Is Close To Player");

	AttackDistance = 125.f;
	bCanAttackPlayerKey = false;
}

//void UBTService_IsCloseToPlayer::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
void UBTService_IsCloseToPlayer::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	// Get references for calculating the distance from the AI and the Player
	ABasic_AIController* ControllerAI = Cast<ABasic_AIController>(OwnerComp.GetAIOwner());
	if (!ControllerAI) return;
	AAI_Basic* AIOwner = Cast<AAI_Basic>(ControllerAI->GetPawn());
	if (!AIOwner) return;
	if (!ControllerAI->GetPlayerTarget()) return;

	// Calculate AI distance from player
	FVector CurrentAILocation = AIOwner->GetActorLocation();
	FVector PlayerLocation = ControllerAI->GetPlayerTarget()->GetActorLocation();
	float AIDistanceFromPlayer = UKismetMathLibrary::Vector_Distance(CurrentAILocation, PlayerLocation);

	// Check if the AI is in range to attack player
	bCanAttackPlayerKey = (AIDistanceFromPlayer <= AttackDistance) ? true : false;

	// Set CanAttachPlayer BB key to bCanAttackPlayerKey
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	BlackboardComp->SetValueAsBool(FName("CanAttackPlayer"), bCanAttackPlayerKey);
}

FString UBTService_IsCloseToPlayer::GetStaticDescription() const
{
	return FString::Printf(TEXT("Bool : %s"), *BlackboardKey.SelectedKeyName.ToString());
}
