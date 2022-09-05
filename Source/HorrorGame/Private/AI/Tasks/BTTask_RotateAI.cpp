// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Tasks/BTTask_RotateAI.h"
#include "AI/Classic/Basic_AIController.h"
#include "AI/Classic/AI_Basic.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "Kismet/KismetMathLibrary.h"

UBTTask_RotateAI::UBTTask_RotateAI()
{
	NodeName = TEXT("Rotate AI");

	//bCreateNodeInstance = true;
	bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_RotateAI::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	//FinishLatentTask(OwnerComp, EBTNodeResult::InProgress);
	return EBTNodeResult::InProgress;
}

void UBTTask_RotateAI::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	ABasic_AIController* ControllerAI = Cast<ABasic_AIController>(OwnerComp.GetAIOwner());
	if (!ControllerAI) return;

	AAI_Basic* AIOwner = Cast<AAI_Basic>(ControllerAI->GetPawn());
	if (!AIOwner) return;

	// Interpolate rotation to target rotation.
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	FRotator TargetRotation = BlackboardComp->GetValueAsRotator(FName("TargetRotation"));
	FRotator CurrentRotation = AIOwner->GetControlRotation();
	FRotator NewRotation = UKismetMathLibrary::RInterpTo(CurrentRotation, TargetRotation, DeltaSeconds, 10.f);

	AIOwner->SetActorRotation(NewRotation);

	bool bReachedTargetRotation = UKismetMathLibrary::EqualEqual_RotatorRotator(CurrentRotation, TargetRotation, 1.f);
	if (bReachedTargetRotation)
	{
		// finish task
		//OwnerComp.OnTaskFinished(this, EBTNodeResult::Succeeded);
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}

FString UBTTask_RotateAI::GetStaticDescription() const
{
	return FString::Printf(TEXT("FRotator : %s"), *BlackboardKey.SelectedKeyName.ToString());
}
