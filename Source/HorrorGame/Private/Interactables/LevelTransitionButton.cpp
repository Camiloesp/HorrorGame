// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactables/LevelTransitionButton.h"
#include "Kismet/GameplayStatics.h"

void ALevelTransitionButton::Interact()
{
	UGameplayStatics::OpenLevel(this, NewLevelName);
}
