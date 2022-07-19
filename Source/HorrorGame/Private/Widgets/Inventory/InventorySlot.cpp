// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Inventory/InventorySlot.h"

#include "Components/Image.h"
#include "Components/Button.h"

UInventorySlot::UInventorySlot(const FObjectInitializer& ObjectInitializer) :Super(ObjectInitializer)
{

}

void UInventorySlot::NativeConstruct()
{
	Index = 0;
	Amount = 0;
}
