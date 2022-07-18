// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactables/LightSwitch.h"
#include "Engine/Light.h"
#include "Components/LightComponent.h"

ALightSwitch::ALightSwitch()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	InteractableMesh->SetRelativeScale3D(FVector(0.15f, 0.05f, 0.2f));
	InteractableMesh->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));
}

// Called when the game starts or when spawned
void ALightSwitch::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ALightSwitch::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ALightSwitch::Interact()
{
	if (Light && Light->GetLightComponent())
	{
		ULightComponent* LightComponent = Light->GetLightComponent();

		// Sets new visibility
		bool bNewVisibility = !( LightComponent->IsVisible() );
		LightComponent->SetVisibility(bNewVisibility);
	}

	Super::Interact(); // Removes reference to player at the end of interaction
}