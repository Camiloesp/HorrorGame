// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_FindRandomRoom.generated.h"

/**
 * 
 */
UCLASS()
class HORRORGAME_API UBTTask_FindRandomRoom : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	
public:
	UBTTask_FindRandomRoom();

protected:

private:

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	virtual FString GetStaticDescription()const override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	FVector TargetLocationKey;
};
