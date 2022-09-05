// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_PullOutOfHiding.generated.h"

/**
 * 
 */
UCLASS()
class HORRORGAME_API UBTTask_PullOutOfHiding : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	
public:
	UBTTask_PullOutOfHiding();

protected:

private:

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	virtual FString GetStaticDescription()const override;
};
