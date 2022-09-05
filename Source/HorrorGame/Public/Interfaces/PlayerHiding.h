// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PlayerHiding.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UPlayerHiding : public UInterface
{
	GENERATED_BODY()
};

class AHideActor;
/**
 * 
 */
class HORRORGAME_API IPlayerHiding
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	/* Did the enemy see the player hiding in a hiding actor? */
	virtual void DidEnemySee(AHideActor* HideActor) = 0;

	virtual void LeftHidingSpot() = 0;
};
