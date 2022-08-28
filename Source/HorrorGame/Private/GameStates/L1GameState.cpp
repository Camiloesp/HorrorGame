// Fill out your copyright notice in the Description page of Project Settings.


#include "GameStates/L1GameState.h"
#include "Widgets/MainHUD.h"
#include "Widgets/ObjectivesWidget.h"

AL1GameState::AL1GameState()
{
}

void AL1GameState::BeginPlay()
{
	Super::BeginPlay();
}

void AL1GameState::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AL1GameState::Initialize(UMainHUD* PlayerHUD)
{
	if (!PlayerHUD) return;

	PlayersHUDRef.Add(PlayerHUD);

	SetObjective(); // todo: if this game was multiplayer, this will be called N times. Have it on beginplay in gamemode?
}

void AL1GameState::CompleteObjectiveCall(FName ObjectiveID, bool SetNextObjectiveImmediately)
{
	FName CurrentObjectiveID = FindCurrentObjective().IDName;

	if(ObjectiveID.IsEqual(CurrentObjectiveID))
	{
		CompleteObjective();

		if (SetNextObjectiveImmediately)
		{
			SetObjective();
		}
		else
		{
			// hide widget?
		}
	}
}

int AL1GameState::GetObjectiveIndex(FObjectiveData ObjectiveToLookFor)
{
	int Index = 0;
	for (FObjectiveData CurrentObjectiveItr : Objectives)
	{
		if (CurrentObjectiveItr.IDName.IsEqual(ObjectiveToLookFor.IDName))
		{
			break;
		}
		Index++;
	}
	return Index;
}

FObjectiveData AL1GameState::FindCurrentObjective()
{
	// The objective we are looking for.
	FObjectiveData ObjetiveTarget;

	for (FObjectiveData CurrentObjective : Objectives)
	{
		// if current objective is not completed
		if (!CurrentObjective.bObjectiveCompleted)
		{
			ObjetiveTarget = CurrentObjective;
			break;
		}
	}

	return ObjetiveTarget;
}

void AL1GameState::SetObjective()
{
	FObjectiveData NewObjective = FindCurrentObjective();

	// update all players widget with the new objective.
	for (UMainHUD* CurrentPlayerHUD: PlayersHUDRef)
	{
		UObjectivesWidget* ObjectiveWidget = CurrentPlayerHUD->GetObjectivesWidget();
		if (ObjectiveWidget)
		{
			ObjectiveWidget->UpdateObjectiveText(NewObjective.ObjectiveTitle);
			ObjectiveWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		}
	}
}

void AL1GameState::CompleteObjective()
{
	FObjectiveData CurrentObjective = FindCurrentObjective();

	int NewIndexObjective = GetObjectiveIndex(CurrentObjective);

	Objectives[NewIndexObjective].bObjectiveCompleted = true;

	// Hide all players widget
	for (UMainHUD* CurrentPlayerHUD : PlayersHUDRef)
	{
		UObjectivesWidget* ObjectiveWidget = CurrentPlayerHUD->GetObjectivesWidget();
		if (ObjectiveWidget)
		{
			ObjectiveWidget->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}
