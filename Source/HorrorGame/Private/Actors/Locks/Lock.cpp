// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Locks/Lock.h"
#include "Actors/Locks/LockDial.h"
#include "Camera/CameraComponent.h"
#include "Components/BoxComponent.h"
#include "Characters/HGCharacter.h"

// Sets default values
ALock::ALock()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	SetRootComponent(RootComp);

	LockMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("LockerMesh"));
	LockMesh->SetupAttachment(RootComp);
	LockMesh->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));

	CameraView = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraView->SetupAttachment(RootComp);
	CameraView->SetRelativeRotation(FRotator(0.f, 180.f, 0.f));
	CameraView->SetRelativeLocation(FVector(15.f, 0.f, -5.f));
	CameraView->FieldOfView = 45.f;
	
	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	BoxCollision->SetupAttachment(RootComp);
	

	// Initialize variables
	bIsUnlocked = false;
	BoxPosition = FVector(0.f, 0.f, 0.f);
	BoxExtent = FVector(32.f, 32.f, 32.f);

	//Edit components post variables initialization
	BoxCollision->SetBoxExtent(BoxExtent);
	BoxCollision->SetRelativeLocation(BoxPosition);

	// Add combination to unlock this lock
	LockCombination.Add(9);
	LockCombination.Add(5);
	LockCombination.Add(1);
	LockCombination.Add(6);
}

// Called when the game starts or when spawned
void ALock::BeginPlay()
{
	Super::BeginPlay();
	
	SpawnDials();
}

// Called every frame
void ALock::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ALock::SpawnDials()
{
	//Spawns dials
	for (int i = 0; i <= 3; i++) // <=3
	{
		if (LockDialClass)
		{
			// Create string name from socket in the LockMesh
			FString SocketString = TEXT("DialBone");
			SocketString.Append(FString::FromInt(i + 1));
			SocketString.Append(TEXT("Socket"));
			FName SocketNameToAttach = FName(*SocketString);

			// Spawn location and rotation
			FVector SpawnLocation = FVector::ZeroVector;
			FRotator SpawnRotation = FRotator::ZeroRotator;
			FActorSpawnParameters SpawnParams;
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

			//LockMesh->GetSocketLocation(SocketNameToAttach);
			//LockMesh->GetSocketRotation(SocketNameToAttach);

			// Spawn dial and add it to array
			ALockDial* SpawnedDial = GetWorld()->SpawnActor<ALockDial>(LockDialClass, SpawnLocation, SpawnRotation, SpawnParams);
			SpawnedDial->SetActorRotation(GetActorRotation()); //test
			SpawnedDial->SetIndex(i);
			Dials.Add(SpawnedDial);

			// Attach spawned actors to this actor mesh
			EAttachmentRule LocationRule = EAttachmentRule::SnapToTarget;
			EAttachmentRule RotationRule = EAttachmentRule::KeepWorld; //KeepWorld
			EAttachmentRule ScaleRule = EAttachmentRule::KeepWorld;
			FAttachmentTransformRules AttachmentRules = FAttachmentTransformRules::FAttachmentTransformRules(LocationRule, RotationRule, ScaleRule, true);
			Dials[i]->AttachToComponent(LockMesh, AttachmentRules, SocketNameToAttach);
		}
	}
}

bool ALock::CheckCode()
{
	// if all of our 4 combinations are equal then bLocalSuccess is always true
	bool bLocalSuccess = false;

	for (int i=0; i<Dials.Num(); i++)
	{
		ALockDial* Dial = Dials[i];

		int CurrentDialNumber = Dial->GetNumber();
		if (LockCombination[i] == CurrentDialNumber)
		{
			bLocalSuccess = true;
		}
		else
		{
			bLocalSuccess = false;
			break; // stop loop
		}
	}

	return bLocalSuccess;
}

void ALock::Interact()
{
	if (!InteractingPlayer) return;
	
	// Look through the camera to mess with dials
	APlayerController* PlayerController = Cast<APlayerController>(InteractingPlayer->GetController());
	if (!PlayerController) return;
	
	//PlayerController->SetViewTargetWithBlend(); 
}
