// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Classic/Basic_AIController.h"

void ABasic_AIController::BeginPlay()
{
	Super::BeginPlay();

	RunBehaviorTree(BT_BasicAI);
}
