// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/ArrowWidget.h"
#include "Components/Button.h"
#include "Actors/Locks/LockDial.h"


bool UArrowWidget::Initialize()
{
	bool bInit = Super::Initialize();

	ArrowButton->OnPressed.AddDynamic(this, &UArrowWidget::ArrowButtonPressed);

	//Initialize variables
	/*
	if ()
	{
		bAddRotation = true;
	}
	else
	{
		bAddRotation = false;
	}*/

	return bInit;
}

void UArrowWidget::ArrowButtonPressed()
{
	if (!LockDialRef) return;

	if (bAddRotation)
	{
		LockDialRef->RotateDial();
	}
	else // Subtracting rotation
	{
		LockDialRef->RotateDialBackwards();
	}
}
