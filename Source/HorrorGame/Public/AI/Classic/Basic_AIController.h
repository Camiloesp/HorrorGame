// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Basic_AIController.generated.h"

/**
 * 
 */
UCLASS()
class HORRORGAME_API ABasic_AIController : public AAIController
{
	GENERATED_BODY()
	
protected:

	virtual void BeginPlay() override;

private:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	UBehaviorTree* BT_BasicAI;
};
