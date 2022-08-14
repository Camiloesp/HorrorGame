// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/HealthComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetMaterialLibrary.h"
#include "Materials/MaterialParameterCollectionInstance.h"

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

	// Just in case we want to start with low health
	UpdateBloodScreen();
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
	UpdateBloodScreen();
}

void UHealthComponent::AddHealth(float Amount)
{
	CurrentHealth += Amount;
	CurrentHealth = UKismetMathLibrary::FClamp(CurrentHealth, MinHealth, MaxHealth);
	UpdateBloodScreen();
}

void UHealthComponent::UpdateBloodScreen()
{
	if (!MaterialParameter) return;// do not forget to assign the MPC_BloodScreen to the HealthComponent from our BP_HGCharacter.

	// Display blood at 70% of health
	float PercentToShowHealth = (MaxHealth - MinHealth) * 0.7f;

	float NewParamValue = UKismetMathLibrary::MapRangeClamped(CurrentHealth, MinHealth, PercentToShowHealth, 0.5f, 1.f);
	UKismetMaterialLibrary::SetScalarParameterValue(this, MaterialParameter, FName(TEXT("Amount")), NewParamValue);
}


