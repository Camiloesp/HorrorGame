// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactables/InteractableBase.h"

// Sets default values
AInteractableBase::AInteractableBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	InteractableMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("InteractableMesh"));
	InteractableMesh->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AInteractableBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AInteractableBase::Tick(float DeltaTime)
{ 
	Super::Tick(DeltaTime);

}

void AInteractableBase::Interact()
{
	UE_LOG(LogTemp, Warning, TEXT("Interacting..."));

	// Super::Interact() should be called at the end of each interactable to remove reference of the player that is interacting.
	InteractingPlayer = nullptr;
}

