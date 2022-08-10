// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ProgressBars.generated.h"


class AHGCharacter;

/**
 * 
 */
UCLASS()
class HORRORGAME_API UProgressBars : public UUserWidget
{
	GENERATED_BODY()
	


private:

	AHGCharacter* PlayerOwner;



protected:
public:

	FORCEINLINE void SetPlayerOwner(AHGCharacter* NewPlayerOwner) { PlayerOwner = NewPlayerOwner; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE AHGCharacter* GetPlayerOwner() const { return PlayerOwner; }
};
