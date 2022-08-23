// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Inventory/Examination.h"
#include "NoteExamination.generated.h"

class UTextRenderComponent;
/**
 * 
 */
UCLASS()
class HORRORGAME_API ANoteExamination : public AExamination
{
	GENERATED_BODY()
	
public:

	ANoteExamination();

protected:


private:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UTextRenderComponent* NoteText;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UTextRenderComponent* BackNoteText;

protected:
public:

	FORCEINLINE UTextRenderComponent* GetNoteText() const { return NoteText; }
	FORCEINLINE UTextRenderComponent* GetBackNoteText() const { return BackNoteText; }

};
