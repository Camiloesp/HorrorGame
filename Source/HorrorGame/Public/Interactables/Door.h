// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interactables/InteractableBase.h"
#include "Components/TimelineComponent.h"
#include "Door.generated.h"


//class UTimelineComponent;
//class FTimeline;
class UBoxComponent;
/**
 * 
 */
UCLASS()
class HORRORGAME_API ADoor : public AInteractableBase
{
	GENERATED_BODY()


public:
	// Sets default values for this character's properties
	ADoor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;




private:

	/* Door mesh */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* Door;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UBoxComponent* AIBoxCollision;

	UFUNCTION()
	void AIOpenDoor(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void AICloseDoor(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
	bool bCanInteract;

	void ActivateInteraction();

protected:

	/* opening the door time line and variables */

	/* Timeline for opening the door */
	FTimeline Timeline;
	/* Keeps track of where we are in our timeline */
	UPROPERTY(EditAnywhere)
	UCurveFloat* CurveFloat;
	/* if the door is closed (start of timeline) then this will be false. */
	bool bIsDoorClosed;
	/* The max rotation this door will open */
	UPROPERTY(EditAnywhere)
	float DoorRotateAngle;
	/* 
	*	This function needs to be binded, therefore the UFUNCTION.
	*	Opens the door.
	* @param Value New value to rotate the door. Will be affected by DoorRotateAngle.
	*/
	UFUNCTION()
	void OpenDoor(float Value);

	bool bDoorOnSameSide;
	void SetDoorOnSameSide();

public:

	/* If true, this door will open towards the player 90 degrees or -90 degrees is opening from the back. If false, it will open in one direction only. */
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
	bool bTwoSidedDoor;

	virtual void Interact() override;

	FORCEINLINE void SetCanInteract(bool bNewInteract) { bCanInteract = bNewInteract; }

};
