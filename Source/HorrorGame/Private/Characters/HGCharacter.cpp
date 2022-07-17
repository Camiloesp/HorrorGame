// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/HGCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
AHGCharacter::AHGCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(RootComponent);
	FollowCamera->SetRelativeLocation(FVector(0.f, 0.f, 60.f));
	FollowCamera->bUsePawnControlRotation = true;


	GetCharacterMovement()->MaxWalkSpeed = 400.f;
	GetCharacterMovement()->JumpZVelocity = 400.f;
	
	TurnRate = 0.4;
}

// Called when the game starts or when spawned
void AHGCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AHGCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AHGCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	/* BIND AXIS */
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &AHGCharacter::LookUp);
	PlayerInputComponent->BindAxis(TEXT("LookRight"), this, &AHGCharacter::LookRight);
	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AHGCharacter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &AHGCharacter::MoveRight);

	/* BIND ACTIONS */
	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Released, this, &ACharacter::StopJumping);
}

void AHGCharacter::LookUp(float Value)
{
	AddControllerPitchInput(TurnRate * Value);
}

void AHGCharacter::LookRight(float Value)
{
	AddControllerYawInput(TurnRate * Value);
}

void AHGCharacter::MoveForward(float Value)
{
	AddMovementInput(GetActorForwardVector(), Value);
}

void AHGCharacter::MoveRight(float Value)
{
	AddMovementInput(GetActorRightVector(), Value);
}

