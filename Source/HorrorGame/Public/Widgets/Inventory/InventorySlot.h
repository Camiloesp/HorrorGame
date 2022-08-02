// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ItemData.h"
#include "InventorySlot.generated.h"


class UButton;
class UImage;
class AHGCharacter;
class UTextBlock;
class UTexture2D;
class UInventoryMenu;

/**
 *
 */
UCLASS()
class HORRORGAME_API UInventorySlot : public UUserWidget
{
	GENERATED_BODY()

public:
	UInventorySlot(const FObjectInitializer& ObjectInitializer);
	
	virtual void NativeConstruct() override;

	virtual bool Initialize() override;

private:

	/* Button to select the current item being held in this Slot */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), meta = (BindWidget))
	UButton* SlotButton;

	/* Slot image for the item */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), meta = (BindWidget))
	UImage* SlotImage;

	/* Icon when button is disabled. (i.e. there is no item in the slot) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UTexture2D* EmptyIcon;

	/* Index for this slot, in the array slot */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), meta = (BindWidget))
	int Index;

	/* Struct that holds the item data (Item image, and name) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), meta = (BindWidget))
	FItemData ItemData;

	/* Amount of items we have in the slot */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), meta = (BindWidget))
	int Amount;

	/* Amount of items we have in the slot to text */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), meta = (BindWidget))
	UTextBlock* AmountText;

	AHGCharacter* PlayerOwnerRef;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UInventoryMenu* InventoryMenuReference;


protected:

	UFUNCTION()
	void SlotButtonPressed();
	UFUNCTION()
	void SlotButtonHovered();
	UFUNCTION()
	void SlotButtonUnHovered();

public:

	/* GETTERS */
	FORCEINLINE int GetIndex() const { return Index; }
	FORCEINLINE UInventoryMenu* GetInventoryMenuReference() const { return InventoryMenuReference; }


	/* SETTERS */
	FORCEINLINE void SetIndex(int NewIndex) { Index = NewIndex; }
	FORCEINLINE void SetPlayerOwnerRef(AHGCharacter* NewPlayerOwnerRef) { PlayerOwnerRef = NewPlayerOwnerRef; }
	FORCEINLINE void SetInventoryMenuReference(UInventoryMenu* NewInventoryMenuReference) { InventoryMenuReference = NewInventoryMenuReference; }

	void UpdateSlot();

};
