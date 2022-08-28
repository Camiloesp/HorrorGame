// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "../ObjectiveData.h"
#include "../Interfaces/ObjectiveCall.h"
#include "L1GameState.generated.h"

class UMainHUD;
/**
 * 
 */
UCLASS()
class HORRORGAME_API AL1GameState : public AGameStateBase, public IObjectiveCall
{
	GENERATED_BODY()
	
public:
	// Sets default values for this character's properties
	AL1GameState();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;


private:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TArray<FObjectiveData> Objectives;

	TArray<UMainHUD*> PlayersHUDRef;

	/* returns the index the ObjectiveToLookFor in our Objectives array */
	int GetObjectiveIndex(FObjectiveData ObjectiveToLookFor);

protected:


	void SetObjective();

	void CompleteObjective();

public:
	/* Finds objective using the IDName in struct FObjectiveData */
	FObjectiveData FindCurrentObjective();

	void Initialize(UMainHUD* PlayerHUD);

	virtual void SetNextObjective(FName ObjectiveID) override;

	virtual void CompleteObjectiveCall(FName ObjectiveID, bool SetNextObjectiveImmediately) override;
};
