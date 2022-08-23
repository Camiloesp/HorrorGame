// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Notes/NoteMaster.h"
#include "Actors/Notes/NoteExamination.h"
#include "Components/TextRenderComponent.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "Characters/HGCharacter.h"
#include "Camera/CameraComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Widgets/Inventory/PickupPrompt.h"
#include "Widgets/NoteExaminationWidget.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ANoteMaster::ANoteMaster()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	SetRootComponent(RootComp);

	PaperMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	PaperMesh->SetupAttachment(RootComp);
	PaperMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	PaperMesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	PaperMesh->SetCollisionObjectType(ECollisionChannel::ECC_WorldStatic);
	PaperMesh->SetRelativeRotation(FRotator(0.f, -90.f, 0.f)); // Front of the actor needs to face in the X-Axis, its forward vector.

	NoteText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("NoteTextRender"));
	NoteText->SetupAttachment(PaperMesh);
	NoteText->SetHorizontalAlignment(EHorizTextAligment::EHTA_Left);
	NoteText->SetVerticalAlignment(EVerticalTextAligment::EVRTA_TextTop);
	NoteText->SetWorldSize(2.f);
	NoteText->SetTextRenderColor(FColor::Black);
	NoteText->SetRelativeRotation(FRotator(90.f, 90.f, 0.f));
	NoteText->SetRelativeLocation(FVector(-10.f, -15.f, 0.001));

	BackNoteText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("BackNoteTextRender"));
	BackNoteText->SetupAttachment(PaperMesh);
	BackNoteText->SetHorizontalAlignment(EHorizTextAligment::EHTA_Left);
	BackNoteText->SetVerticalAlignment(EVerticalTextAligment::EVRTA_TextTop);
	BackNoteText->SetWorldSize(2.f);
	BackNoteText->SetTextRenderColor(FColor::Black);
	BackNoteText->SetRelativeRotation(FRotator(-90.f, 360.f, -90.f));
	BackNoteText->SetRelativeLocation(FVector(10.f, -15.f, -0.11));

	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Collision"));
	SphereCollision->SetupAttachment(PaperMesh);
	SphereCollision->SetSphereRadius(500.f);

	PromptWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("Propmt Widget"));
	PromptWidget->SetRelativeScale3D(FVector(0.03f, 0.03f, 0.03f));
	PromptWidget->SetRelativeLocation(FVector(0.f, 0.f, 20.f));
	PromptWidget->SetupAttachment(RootComp);
	PromptWidget->SetVisibility(false);

	bUpdateActor = false;
	WidgetDistanceAboveMesh = 25.f;
	PickUpDistance = 150.f;

}

// Called when the game starts or when spawned
void ANoteMaster::BeginPlay()
{
	Super::BeginPlay();

	NoteText->SetText(TextNote);
	BackNoteText->SetText(BackTextNote);

	SphereCollision->OnComponentBeginOverlap.AddDynamic(this, &ANoteMaster::ShowPromptWidget);
	SphereCollision->OnComponentEndOverlap.AddDynamic(this, &ANoteMaster::HidePromptWidget);

	TArray<AActor*> NoteExaminationActors; // Only one in the level
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), NoteExaminationClass, NoteExaminationActors);
	NoteExaminationRef = Cast<ANoteExamination>(NoteExaminationActors[0]);
}

// Called every frame
void ANoteMaster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	FVector Distance;
	Distance.X = PaperMesh->GetComponentLocation().X;
	Distance.Y = PaperMesh->GetComponentLocation().Y;
	Distance.Z = PaperMesh->GetComponentLocation().Z + WidgetDistanceAboveMesh;
	PromptWidget->SetWorldLocation(Distance);

	if (bUpdateActor)
	{
		if (!PickerCharacter) return;

		// if our linetrace detects something in between the PromptWidget and the player camera, do not display the prompt
		FHitResult HitResult;
		const FVector Start = GetActorLocation();
		const FVector End = PickerCharacter->GetFollowCamera()->GetComponentLocation();
		GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECollisionChannel::ECC_Visibility);

		// Show PromptWidget
		if (!HitResult.bBlockingHit)
		{
			PromptWidget->SetVisibility(true);

			const FVector WidgetStart = PromptWidget->GetComponentLocation();
			const FVector CharacterEnd = PickerCharacter->GetFollowCamera()->GetComponentLocation();
			FRotator NewRotation = UKismetMathLibrary::FindLookAtRotation(WidgetStart, CharacterEnd);
			PromptWidget->SetWorldRotation(NewRotation);
		}
		else
		{
			PromptWidget->SetVisibility(false);
			DisableInput(Cast<APlayerController>(PickerCharacter->GetController()));
		}

		// Length of line trace
		double LinetraceLength = UKismetMathLibrary::VSize(HitResult.TraceStart - HitResult.TraceEnd);
		if (LinetraceLength <= PickUpDistance)
		{
			// Display pick up 'E'
			UPickupPrompt* PickupPrompt = Cast<UPickupPrompt>(PromptWidget->GetUserWidgetObject());
			if (PickupPrompt)
			{
				PickupPrompt->TogglePrompt(true);
			}

			EnableInput(Cast<APlayerController>(PickerCharacter->GetController()));
		}
		else
		{
			// Display pick up Arrow
			UPickupPrompt* PickupPrompt = Cast<UPickupPrompt>(PromptWidget->GetUserWidgetObject());
			if (PickupPrompt)
			{
				PickupPrompt->TogglePrompt(false);
			}
			DisableInput(Cast<APlayerController>(PickerCharacter->GetController()));
		}
	}
}

void ANoteMaster::Interact()
{
	if (!InteractingPlayer || !NoteExaminationWidgetClass) return;
	if (!NoteExaminationRef) return;

	InteractingPlayer->bReadingNote = true;

	UNoteExaminationWidget* ExaminationWidget = CreateWidget<UNoteExaminationWidget>(GetWorld(), NoteExaminationWidgetClass);
	if (ExaminationWidget)
	{
		ExaminationWidget->SetNoteExamination(NoteExaminationRef);
		ExaminationWidget->SetOwnerCharacter(InteractingPlayer);
		ExaminationWidget->UpdateWidget(this);
		ExaminationWidget->AddToViewport();
	}

}

void ANoteMaster::ShowPromptWidget(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		PickerCharacter = Cast<AHGCharacter>(OtherActor);
		if (PickerCharacter)
		{
			PromptWidget->SetVisibility(true);
			bUpdateActor = true;
		}
	}
}

void ANoteMaster::HidePromptWidget(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor)
	{
		PickerCharacter = Cast<AHGCharacter>(OtherActor);
		if (PickerCharacter)
		{
			PromptWidget->SetVisibility(false);
			bUpdateActor = false;

			DisableInput(Cast<APlayerController>(PickerCharacter->GetController()));
			//PickerCharacter = nullptr;
		}
	}
}

