// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "NoteExaminationWidget.generated.h"

class ANoteExamination;
class ANoteMaster;
class UImage;
class AHGCharacter;
/**
 * 
 */
UCLASS()
class HORRORGAME_API UNoteExaminationWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	virtual bool Initialize() override;

private:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	ANoteExamination* NoteExaminationRef;

	/* Image used to detect the user mouse input to rotate the examination actor */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), meta = (BindWidget))
	UImage* DragDetection;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	AHGCharacter* OwnerCharacter;

	bool bRotateMesh;

	ANoteMaster* CurrentNoteActor;

	/* Cursor position when rotating examination actor */
	FVector2D MouseLocation;

protected:

	/* detects when our mouse button is down, this function will be called. */
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	/* detects when our mouse button is released, this function will be called */
	virtual FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	/* detects when our mouse is moving, this function will be called */
	virtual FReply NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	void DisableReaderInput();
	void EnableReaderInput();

public:

	void UpdateWidget(ANoteMaster* NoteActor);

	UFUNCTION()
	void CloseExamination();

	FORCEINLINE void SetNoteExamination(ANoteExamination* NewNoteExamination) { NoteExaminationRef = NewNoteExamination; }
	FORCEINLINE void SetOwnerCharacter(AHGCharacter* NewOwnerCharacter) { OwnerCharacter = NewOwnerCharacter; }
};
