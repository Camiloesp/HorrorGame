// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interactables/InteractableBase.h"
#include "Door.generated.h"

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

protected:
public:

	virtual void Interact() override;

};
