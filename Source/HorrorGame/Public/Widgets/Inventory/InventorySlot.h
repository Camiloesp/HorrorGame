// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventorySlot.generated.h"


class UButton;
class UImage;

/**
 *
 */
UCLASS()
class HORRORGAME_API UInventorySlot : public UUserWidget
{
	GENERATED_BODY()

public:
	UInventorySlot(const FObjectInitializer& ObjectInitializer);
	
private:

	/* Button to select the current item being held in this Slot */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), meta = (BindWidget))
	UButton* SlotButton;

	/* Slot image for the item */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), meta = (BindWidget))
	UImage* SlotImage;

	/* Index for this slot, in the array slot */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), meta = (BindWidget))
	int Index;

protected:
public:

	/* GETTERS */
	FORCEINLINE int GetIndex() const { return Index; }


	/* SETTERS */
	FORCEINLINE void SetIndex(int NewIndex) { Index = NewIndex; }

};
