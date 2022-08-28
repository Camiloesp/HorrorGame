// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/JumpscareActors/DoorwayJumpscare.h"
//#include "Animation/AnimationAsset.h"
#include "Components/ArrowComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Interfaces/ObjectiveCall.h"
#include "GameStates/L1GameState.h"

ADoorwayJumpscare::ADoorwayJumpscare()
{

	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(GetRootComponent());
	Mesh->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));

	EndPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("End Point"));
	EndPoint->SetupAttachment(GetRootComponent());
	EndPoint->SetRelativeLocation(FVector(500.f, 0.f, 0.f));

	bCalledJumpscare = false;
	AnimationInterpSpeed = 300.f;
	SecondsToDestroyThisEvent = 1.f;
	
	//SetActorTickEnabled(false);
}

void ADoorwayJumpscare::BeginPlay()
{
	Super::BeginPlay();
}

void ADoorwayJumpscare::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
}

void ADoorwayJumpscare::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bCalledJumpscare)
	{
		FVector CurrentValue = Mesh->GetComponentLocation();
		FVector Target = EndPoint->GetComponentLocation();
		FVector NewLocation = UKismetMathLibrary::VInterpTo_Constant(CurrentValue, Target, DeltaTime, AnimationInterpSpeed);

		Mesh->SetWorldLocation(NewLocation);
	}
}

void ADoorwayJumpscare::TriggerJumpscare(AActor* OverlappingActor)
{
	Super::TriggerJumpscare(OverlappingActor);

	bCalledJumpscare = true;
	/*
	* OPTIONAL:
	* - Disable movement.
	* - Force character face the event direction.
	*/

	FTimerHandle DestroyTriggerBoxTimer;
	GetWorld()->GetTimerManager().SetTimer(DestroyTriggerBoxTimer, this, &ADoorwayJumpscare::DestroyJumpscare, SecondsToDestroyThisEvent, false);
}

void ADoorwayJumpscare::DestroyJumpscare()
{
	// Let GameState know this jumpscare was triggered to see if it activates an objective
	IObjectiveCall* GameState = Cast<IObjectiveCall>(GetWorld()->GetGameState());
	if (GameState)
	{
		QuestID = FName("InvestigateNoise");
		GameState->SetNextObjective(QuestID); //InvestigateNoise
	}


	Destroy();
}

void ADoorwayJumpscare::CompleteObjectiveCall(FName ObjectiveID, bool SetNextObjectiveImmediately)
{
}

void ADoorwayJumpscare::SetNextObjective(FName ObjectiveID)
{
}
