// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Movement.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Characters/HGCharacter.h"

// Sets default values for this component's properties
UMovement::UMovement()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
	SprintSpeed = 500.f;
	WalkSpeed = 250.f;
	MaxStamina = 100.f;
	MinStamina = 0.f;
	CurrentStamina = MaxStamina;
	CrouchSpeed = 150.f;
	CrouchHalfHeight = 44.f;
	bIsCrouching = false;
}


// Called when the game starts
void UMovement::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UMovement::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UMovement::Initialize(ACharacter* Character)
{
	if (Character)
	{
		PlayerOwnerRef = Character;
		PlayerOwnerRef->GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	}
}

void UMovement::SprintTimer()
{
	// if player velocity is > 10
	if (PlayerOwnerRef->GetVelocity().Length() > 10.f)
	{
		CurrentStamina -= 1.f;
		CurrentStamina = FMath::Clamp(CurrentStamina, MinStamina, MaxStamina);

		UE_LOG(LogTemp, Warning, TEXT("UMovement : Current Stamina is %f"), CurrentStamina);

		if (CurrentStamina <= MinStamina)
		{
			StopSprint();
		}
	}
	else
	{
		StopSprint();
	}
}

void UMovement::RegenerateStamina()
{
	if ( !GetWorld()->GetTimerManager().IsTimerActive(DepletesStaminaTimer) )
	{
		CurrentStamina += 1.f;
		CurrentStamina = FMath::Clamp(CurrentStamina, MinStamina, MaxStamina);

		if (CurrentStamina >= MaxStamina)
		{
			GetWorld()->GetTimerManager().ClearTimer(RegenerateStaminaTimer);
		}
		else
		{
			GetWorld()->GetTimerManager().SetTimer(RegenerateStaminaTimer, this, &UMovement::RegenerateStamina, 0.03f, false);
		}
	}
}

void UMovement::StopSprint()
{
	if (bIsCrouching) return;

	GetWorld()->GetTimerManager().ClearTimer(DepletesStaminaTimer);

	SetPlayerMaxWalkSpeed(WalkSpeed);

	GetWorld()->GetTimerManager().SetTimer(RegenerateStaminaTimer, this, &UMovement::RegenerateStamina, 5.f, false);
}

void UMovement::StartSprint()
{
	if (bIsCrouching)
	{
		EndCrouch();
	}

	// If stamina is > 0 AND player velocity is > 10
	if ( (CurrentStamina > MinStamina) && (PlayerOwnerRef->GetVelocity().Length() > 10.f) )
	{
		// if stamina is regenerating, stop it.
		if (GetWorld()->GetTimerManager().IsTimerActive(RegenerateStaminaTimer))
		{
			GetWorld()->GetTimerManager().ClearTimer(RegenerateStaminaTimer);
		}


		SetPlayerMaxWalkSpeed(SprintSpeed);

		GetWorld()->GetTimerManager().SetTimer(DepletesStaminaTimer, this, &UMovement::SprintTimer, 0.1f, true);
	}
}

void UMovement::SetPlayerMaxWalkSpeed(float NewSpeed)
{
	if (PlayerOwnerRef)
	{
		PlayerOwnerRef->GetCharacterMovement()->MaxWalkSpeed = NewSpeed;
	}
}

void UMovement::StartCrouch()
{
	if (GetWorld()->GetTimerManager().IsTimerActive(DepletesStaminaTimer))
	{
		GetWorld()->GetTimerManager().ClearTimer(DepletesStaminaTimer);
	}

	SetPlayerMaxWalkSpeed(CrouchSpeed);

	if (PlayerOwnerRef)
	{
		AHGCharacter* PlayerCharacter = Cast<AHGCharacter>(PlayerOwnerRef);
		if (PlayerCharacter)
		{
			PlayerCharacter->ShortenPlayerCapsule();
			bIsCrouching = true;
		}
	}
}

void UMovement::EndCrouch()
{
	SetPlayerMaxWalkSpeed(WalkSpeed);

	if (PlayerOwnerRef)
	{
		AHGCharacter* PlayerCharacter = Cast<AHGCharacter>(PlayerOwnerRef);
		if (PlayerCharacter)
		{
			PlayerCharacter->LengthenPlayerCapsule();
			bIsCrouching = false;
		}
	}
}
