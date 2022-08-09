// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/HealthComponent.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
	MinHealth = 0.f;
	MaxHealth = 100.f;
	CurrentHealth = MaxHealth;
}


// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UHealthComponent::RemoveHealth(float Amount)
{
	CurrentHealth -= Amount;
	CurrentHealth = UKismetMathLibrary::FClamp(CurrentHealth, MinHealth, MaxHealth);
}

void UHealthComponent::AddHealth(float Amount)
{
	CurrentHealth += Amount;
	CurrentHealth = UKismetMathLibrary::FClamp(CurrentHealth, MinHealth, MaxHealth);
}


