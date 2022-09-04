// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/MainHUD.h"
#include "Widgets/Inventory/InventoryGrid.h"
#include "Widgets/ProgressBars.h"
#include "Characters/HGCharacter.h"

void UMainHUD::NativeConstruct()
{
	//InventoryGrid->
}

bool UMainHUD::Initialize()
{
	/*  
				EVENT Construct IS THE BEGINPLAY FOR WIDGETS............
	
	*/

	bool bSuccess = Super::Initialize();

	/*
	PlayerOwner = Cast<AHGCharacter>(GetOwningPlayerPawn());
	if (PlayerOwner)
	{
		ProgressBars->SetPlayerOwner(PlayerOwner);
	}
	*/

	return bSuccess;
}
