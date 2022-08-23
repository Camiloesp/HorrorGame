// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Notes/NoteExamination.h"
#include "Components/TextRenderComponent.h"

ANoteExamination::ANoteExamination()
{
	NoteText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("NoteTextRender"));
	NoteText->SetupAttachment(GetMeshComponent());
	NoteText->SetHorizontalAlignment(EHorizTextAligment::EHTA_Left);
	NoteText->SetVerticalAlignment(EVerticalTextAligment::EVRTA_TextTop);
	NoteText->SetWorldSize(2.f);
	NoteText->SetTextRenderColor(FColor::Black);
	NoteText->SetRelativeRotation(FRotator(90.f, 90.f, 0.f));
	NoteText->SetRelativeLocation(FVector(-10.f, -15.f, 0.001));

	BackNoteText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("BackNoteTextRender"));
	BackNoteText->SetupAttachment(GetMeshComponent());
	BackNoteText->SetHorizontalAlignment(EHorizTextAligment::EHTA_Left);
	BackNoteText->SetVerticalAlignment(EVerticalTextAligment::EVRTA_TextTop);
	BackNoteText->SetWorldSize(2.f);
	BackNoteText->SetTextRenderColor(FColor::Black);
	BackNoteText->SetRelativeRotation(FRotator(-90.f, 360.f, -90.f));
	BackNoteText->SetRelativeLocation(FVector(10.f, -15.f, -0.11));
}