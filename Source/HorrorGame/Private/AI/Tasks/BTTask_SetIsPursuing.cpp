// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Tasks/BTTask_SetIsPursuing.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_SetIsPursuing::UBTTask_SetIsPursuing()
{
	NodeName = TEXT("Set is pursuing");

	bNewIsPursuing = false;
}

EBTNodeResult::Type UBTTask_SetIsPursuing::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	BlackboardComp->SetValueAsBool(FName("IsPursuingPlayer"), bNewIsPursuing);

	return EBTNodeResult::Succeeded;
}

FString UBTTask_SetIsPursuing::GetStaticDescription() const
{
	return FString::Printf(TEXT("Vector : %s"), *BlackboardKey.SelectedKeyName.ToString());
}
