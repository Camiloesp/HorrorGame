// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ObjectiveCall.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UObjectiveCall : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class HORRORGAME_API IObjectiveCall
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION()
	virtual void CompleteObjectiveCall(FName ObjectiveID, bool SetNextObjectiveImmediately) = 0;

	UFUNCTION()
	virtual void SetNextObjective(FName ObjectiveID) = 0;
};
