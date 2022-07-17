// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GrabInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UGrabInterface : public UInterface
{
	GENERATED_BODY()
};

class AHGCharacter;

/**
 * 
 */
class HORRORGAME_API IGrabInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION()
	virtual void GrabObject() = 0; // = 0 tells the compiler to not look for the definition of this function in the .cpp

	UFUNCTION()
	virtual void ReleaseObject() = 0;

	/* Player who is interacting with the interactable. */
	AHGCharacter* GrabberPlayer;
};
