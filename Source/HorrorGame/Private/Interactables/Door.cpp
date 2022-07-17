// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactables/Door.h"

ADoor::ADoor()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Door = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorFrame"));
	Door->SetupAttachment(InteractableMesh);
	Door->SetRelativeLocation(FVector(0.f, 45.f, 0.f));
}

void ADoor::BeginPlay()
{
}

void ADoor::Tick(float DeltaTime)
{
}

void ADoor::Interact()
{
	UE_LOG(LogTemp, Warning, TEXT("Interacting... With a door"));
}
