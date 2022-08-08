// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/FlashlightComponent.h"
#include "Components/SpotLightComponent.h"
#include "Characters/HGCharacter.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values for this component's properties
UFlashlightComponent::UFlashlightComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
	MaxBatteryLevel = 100.f;
	MinBatteryLevel = 0.f;
	CurrentBatteryLevel = 50.f;
	DepletionAmount = 1.f;
	DepletionSpeed = 30.f;
}


// Called when the game starts
void UFlashlightComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UFlashlightComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UFlashlightComponent::DepleatBatteryLife()
{
	/* Make sure new battery level does not go above MaxBatteryLevel */
	float NewBatteryLevel = CurrentBatteryLevel - DepletionAmount;
	CurrentBatteryLevel = UKismetMathLibrary::FClamp(NewBatteryLevel, MinBatteryLevel, MaxBatteryLevel);

	if (CurrentBatteryLevel <= MinBatteryLevel)
	{
		GetWorld()->GetTimerManager().ClearTimer(DepleateTimerHandler);
		FlashlightLight->SetVisibility(false);
	}
}

void UFlashlightComponent::AddBatteryLife(float Amount)
{
	/* Make sure new battery level does not go above MaxBatteryLevel */
	float NewBatteryLevel = CurrentBatteryLevel + Amount;
	CurrentBatteryLevel = UKismetMathLibrary::FClamp(NewBatteryLevel, MinBatteryLevel, MaxBatteryLevel);
}

void UFlashlightComponent::ToggleFlashlight()
{
	//bool bIsFlashlightOn = !(FlashlightLight->IsVisible());
	//FlashlightLight->SetVisibility(bIsFlashlightOn);

	if(FlashlightLight->IsVisible())
	{
		FlashlightLight->SetVisibility(false);

		GetWorld()->GetTimerManager().ClearTimer(DepleateTimerHandler);
	}
	else
	{
		if (CurrentBatteryLevel > MinBatteryLevel)
		{
			FlashlightLight->SetVisibility(true);
			// Calls DepleatBatteryLife() every DepletionSpeed seconds.
			GetWorld()->GetTimerManager().SetTimer(DepleateTimerHandler, this, &UFlashlightComponent::DepleatBatteryLife, DepletionSpeed, true);
		}
	}
}

void UFlashlightComponent::Initialize(AHGCharacter* OwnerRef)
{
	PlayerRef = OwnerRef;
	SetFlashlightLight(PlayerRef->GetFlashlight());
}

