// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Locks/Lock.h"
#include "Actors/Locks/LockDial.h"

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
}

// Called when the game starts or when spawned
void ALock::BeginPlay()
{
	Super::BeginPlay();
	
	//Spawns dials
	for (int i=0; i<=3; i++)
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

			// Spawn dial and add it to array
			ALockDial* SpawnedDial = GetWorld()->SpawnActor<ALockDial>(LockDialClass, SpawnLocation, SpawnRotation, SpawnParams);
			SpawnedDial->SetIndex(i);
			Dials.Add(SpawnedDial);

			// Attach spawned actors to this actor mesh
			EAttachmentRule LocationRule = EAttachmentRule::SnapToTarget;
			EAttachmentRule RotationRule = EAttachmentRule::KeepWorld;
			EAttachmentRule ScaleRule = EAttachmentRule::KeepWorld;
			FAttachmentTransformRules AttachmentRules = FAttachmentTransformRules::FAttachmentTransformRules(LocationRule, RotationRule, ScaleRule, true);
			Dials[i]->AttachToComponent(LockMesh, AttachmentRules, SocketNameToAttach);
		}
	}
}

// Called every frame
void ALock::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

