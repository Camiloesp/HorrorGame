// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTService_IsCloseToPlayer.generated.h"

/**
 * 
 */
UCLASS()
class HORRORGAME_API UBTService_IsCloseToPlayer : public UBTService_BlackboardBase
{
	GENERATED_BODY()
	
public:
	UBTService_IsCloseToPlayer();

protected:

private:

	//virtual void CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	virtual FString GetStaticDescription()const override;

	/* Distance the AI needs to be from the player to start attacking */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	float AttackDistance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	bool bCanAttackPlayerKey;
};
