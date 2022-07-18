// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interactables/InteractableBase.h"
#include "LightSwitch.generated.h"

class ALight;
/**
 * 
 */
UCLASS()
class HORRORGAME_API ALightSwitch : public AInteractableBase
{
	GENERATED_BODY()
	
public:
	// Sets default values for this character's properties
	ALightSwitch();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:

	/* Light the switch will turn on/off */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	ALight* Light;

protected:
public:

	virtual void Interact() override;

};
