// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Inventory/Pickups/Pickup_FlashlightBattery.h"
#include "GameStates/L1GameState.h"
#include "Interfaces/ObjectiveCall.h"

APickup_FlashlightBattery::APickup_FlashlightBattery()
{
}

void APickup_FlashlightBattery::BeginPlay()
{
	Super::BeginPlay();
}

void APickup_FlashlightBattery::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

bool APickup_FlashlightBattery::PickUp()
{
	bool bParentPickupSuccess = Super::PickUp();

	if (bParentPickupSuccess)
	{
		//AL1GameState* GameState = Cast<AL1GameState>(GetWorld()->GetGameState());
		IObjectiveCall* GameState = Cast<IObjectiveCall>(GetWorld()->GetGameState());
		if (!GameState) return false;
		GameState->CompleteObjectiveCall(QuestID, true);

		bParentPickupSuccess = true;
	}
	else
	{
		bParentPickupSuccess = false;
	}


	return bParentPickupSuccess;
}
