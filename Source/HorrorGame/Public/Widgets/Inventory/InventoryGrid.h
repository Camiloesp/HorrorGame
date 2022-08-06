// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryGrid.generated.h"


class UInventorySlot;
class UUniformGridPanel;
class AHGCharacter;
class UInventoryMenu;
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
	AHGCharacter* OwnerPlayerRef;

	TArray<UInventorySlot*> SlotsArray;

	int SlotsPerRow;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UInventoryMenu* InventoryMenuReference;

protected:
public:

	void AddMoreSlots(int Amount);

	/* GETTERS */
	FORCEINLINE int GetSlotsPerRow() const { return SlotsPerRow; }
	FORCEINLINE TArray<UInventorySlot*> GetSlotsArray() const { return SlotsArray; }
	FORCEINLINE UInventoryMenu* GetInventoryMenuReference() const { return InventoryMenuReference; }
	FORCEINLINE UUniformGridPanel* GetInventoryGridPanel() const { return InventoryGridPanel; }

	/* SETTERS */
	FORCEINLINE void SetSlotsPerRow(int NewSlotsPerRow) { SlotsPerRow = NewSlotsPerRow; }
	FORCEINLINE void SetInventoryMenuReference(UInventoryMenu* NewInventoryMenuReference) { InventoryMenuReference = NewInventoryMenuReference; }
};
