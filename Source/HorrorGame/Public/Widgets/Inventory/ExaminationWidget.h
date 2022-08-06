// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ExaminationWidget.generated.h"

class UTextBlock;
class UImage;
class UInventoryComponent;
class AHGCharacter;
/**
 * 
 */
UCLASS()
class HORRORGAME_API UExaminationWidget : public UUserWidget
{
	GENERATED_BODY()
	

public:

	virtual bool Initialize() override;

private:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), meta = (BindWidget))
	UTextBlock* ItemNameText;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), meta = (BindWidget))
	UTextBlock* ItemDescriptionText;

	/* Image used to detect the user mouse input to rotate the examination actor */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), meta = (BindWidget))
	UImage* DragDetection;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UInventoryComponent* InventoryCompRef;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	AHGCharacter* OwnerCharacter;

	bool bRotateMesh;

	/* Cursor position when rotating examination actor */
	FVector2D MouseLocation;

protected:

	/* detects when our mouse button is down, this function will be called. */
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	/* detects when our mouse button is released, this function will be called */
	virtual FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	/* detects when our mouse is moving, this function will be called */
	virtual FReply NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

public:

	/*
	* Updates Name and description
	* @param Index Index to the element from our inventory we want to inspect.
	*/
	void UpdateWidget(int Index);

	UFUNCTION()
	void CloseExamination();

	FORCEINLINE void SetInventoryCompRef(UInventoryComponent* NewInventoryCompRef) { InventoryCompRef = NewInventoryCompRef; }
	FORCEINLINE void SetOwnerCharacter(AHGCharacter* NewOwnerCharacter) { OwnerCharacter = NewOwnerCharacter; }

	FORCEINLINE UInventoryComponent* GetInventoryCompRef() const { return InventoryCompRef; }
	FORCEINLINE AHGCharacter* GetOwnerCharacter() const { return OwnerCharacter; }
};
