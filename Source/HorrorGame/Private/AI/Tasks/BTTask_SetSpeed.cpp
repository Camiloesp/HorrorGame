// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Tasks/BTTask_SetSpeed.h"
#include "AI/Classic/AI_Basic.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AIController.h"

UBTTask_SetSpeed::UBTTask_SetSpeed()
{
	NodeName = TEXT("Set AI movement speed");

	MaxWalkSpeed = 600.f;
}

EBTNodeResult::Type UBTTask_SetSpeed::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	AAI_Basic* AIOwner = Cast<AAI_Basic>(OwnerComp.GetAIOwner()->GetPawn());
	if (!AIOwner) return EBTNodeResult::Failed;

	AIOwner->GetCharacterMovement()->MaxWalkSpeed = MaxWalkSpeed;

	return EBTNodeResult::Succeeded;
}

FString UBTTask_SetSpeed::GetStaticDescription() const
{
	return FString::Printf(TEXT("Float : %s"), *BlackboardKey.SelectedKeyName.ToString());
}
