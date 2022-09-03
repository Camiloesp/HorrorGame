// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Tasks/BTTask_SetIsPursuing.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AI/Classic/Basic_AIController.h"

UBTTask_SetIsPursuing::UBTTask_SetIsPursuing()
{
	NodeName = TEXT("Set is pursuing");

	bNewIsPursuing = false;
}

EBTNodeResult::Type UBTTask_SetIsPursuing::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ABasic_AIController* ControllerAI = Cast<ABasic_AIController>(OwnerComp.GetAIOwner());
	if (!ControllerAI) return EBTNodeResult::Failed;

	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	BlackboardComp->SetValueAsBool(FName("IsPursuingPlayer"), bNewIsPursuing);

	// Change AlreadyPlayedJumpscareSound to false
	if (ControllerAI->AlreadyPlayedJumpscareSound())
	{
		ControllerAI->SetAlreadyPlayedJumpscareSound(false);
	}

	return EBTNodeResult::Succeeded;
}

FString UBTTask_SetIsPursuing::GetStaticDescription() const
{
	return FString::Printf(TEXT("Vector : %s"), *BlackboardKey.SelectedKeyName.ToString());
}
