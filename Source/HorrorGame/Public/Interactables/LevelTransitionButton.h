// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interactables/InteractableBase.h"
#include "LevelTransitionButton.generated.h"

/**
 * 
 */
UCLASS()
class HORRORGAME_API ALevelTransitionButton : public AInteractableBase
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName NewLevelName;

	virtual void Interact() override;
};
