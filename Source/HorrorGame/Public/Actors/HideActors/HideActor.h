// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../../Interfaces/Interaction.h"
#include "HideActor.generated.h"

class UArrowComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FFinishMovingDelegate);


UCLASS()
class HORRORGAME_API AHideActor : public AActor, public IInteraction
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHideActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


private:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USceneComponent* RootComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* Mesh;

	/* Position of the player inside the locker */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UArrowComponent* HidingPosition;
	/* Position of the player exiting the locker */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UArrowComponent* ExitPosition;

protected:

	/* Moves our character to PlayerPosition component location */
	void MoveCharacter();
	/* Called once we finish moving to our target locations (Hiding location, or exit location) */
	void FinishMoving();

	FTimerHandle HidingMovementTimer;

public:

	FFinishMovingDelegate FinishMovingDelegate;

	virtual void Interact() override;

	FORCEINLINE UStaticMeshComponent* GetMesh() const { return Mesh; }
	FORCEINLINE UArrowComponent* GetHidingPosition() const { return HidingPosition; }
	FORCEINLINE UArrowComponent* GetExitPosition() const { return ExitPosition; }

};
