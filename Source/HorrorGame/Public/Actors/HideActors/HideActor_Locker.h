// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/HideActors/HideActor.h"
#include "Components/TimelineComponent.h"
#include "HideActor_Locker.generated.h"

/**
 * 
 */
UCLASS()
class HORRORGAME_API AHideActor_Locker : public AHideActor
{
	GENERATED_BODY()
	
public:

	AHideActor_Locker();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;



private:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* LockerDoorMesh;

	/* Timeline for opening the locker door */
	FTimeline Timeline; // OpenLockerDoorTimeline
	/* Keeps track of where we are in our timeline */
	UPROPERTY(EditAnywhere)
	UCurveFloat* CurveFloat;

protected:

	/* Opens locker door by adding Value as rotation */
	UFUNCTION()
	void OpenLockerDoor(float Value);

public:

	virtual void Interact() override;
};