// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryGrid.generated.h"


class UInventorySlot;
class UUniformGridPanel;
/**
 * 
 */
UCLASS()
class HORRORGAME_API UInventoryGrid : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UInventoryGrid(const FObjectInitializer& ObjectInitializer);

	virtual bool Initialize() override;

protected:



private:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), meta = (BindWidget))
	UUniformGridPanel* InventoryGridPanel;

	/* Class for InventorySlots */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UUserWidget> InventorySlotsClass;

	/* Player that owns this widget */
	ACharacter* OwnerPlayerRef;

	TArray<UInventorySlot*> SlotsArray;

	int SlotsPerRow;

protected:
public:
	/* GETTERS */
	FORCEINLINE int GetSlotsPerRow() const { return SlotsPerRow; }

	/* SETTERS */
	FORCEINLINE void SetSlotsPerRow(int NewSlotsPerRow) { SlotsPerRow = NewSlotsPerRow; }
};
