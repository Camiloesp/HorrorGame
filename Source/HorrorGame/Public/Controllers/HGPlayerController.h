// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "HGPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class HORRORGAME_API AHGPlayerController : public APlayerController
{
	GENERATED_BODY()
	

public:
	AHGPlayerController();

protected:

	virtual void BeginPlay() override;

private:

	/* How many inventory slots will the player have */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	int InventorySlots;

protected:
public:

	FORCEINLINE int GetInventorySlots() const { return InventorySlots; }
};
