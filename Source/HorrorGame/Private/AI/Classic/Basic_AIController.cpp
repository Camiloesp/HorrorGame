// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Classic/Basic_AIController.h"
//#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "Characters/HGCharacter.h"

ABasic_AIController::ABasic_AIController()
{

	//UAIPerceptionComponent* Perception;
	Perception = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Perception Component"));

	//Creating Sight sense
	SightSense = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Sense Config"));
	SightSense->SightRadius = 1'000.f;
	SightSense->LoseSightRadius = 1'250.f;
	SightSense->PeripheralVisionAngleDegrees = 75.f;

	// Adding sense to perception config
	Perception->ConfigureSense(*SightSense);
	Perception->SetDominantSense(SightSense->GetSenseImplementation());

	bCanSeePlayer = false;
	bAlreadyPlayedJumpscareSound = false;
}

void ABasic_AIController::BeginPlay()
{
	Super::BeginPlay();

	RunBehaviorTree(BT_BasicAI);

	Perception->OnTargetPerceptionUpdated.AddDynamic(this, &ABasic_AIController::OnPerception);
}

void ABasic_AIController::OnPerception(AActor* Actor, FAIStimulus Stimulos)
{
	PlayerTarget = Cast<AHGCharacter>(Actor);
	if (!PlayerTarget) return;

	if(Stimulos.WasSuccessfullySensed())
	{
		bCanSeePlayer = true;
		Blackboard->SetValueAsBool(FName("CanSeePlayer"), bCanSeePlayer);

		Blackboard->SetValueAsBool(FName("IsPursuingPlayer"), bCanSeePlayer);
	}
	else
	{
		bCanSeePlayer = false;
		Blackboard->SetValueAsBool(FName("CanSeePlayer"), bCanSeePlayer);
		//IsPursuingPlayer will stay true even after we lose player, so the AI can investigate.
	}
}
