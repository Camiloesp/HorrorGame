// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/AI_Helpers/RoomTarget.h"
#include "Components/BillboardComponent.h"
#include "Components/SphereComponent.h"

// Sets default values
ARoomTarget::ARoomTarget()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(RootComp);

	Billboard = CreateDefaultSubobject<UBillboardComponent>(TEXT("BillboardSymbol"));
	Billboard->SetupAttachment(GetRootComponent());

	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	SphereCollision->SetupAttachment(Billboard);
	SphereCollision->SetSphereRadius(415.f);
	// USphereComponent * SphereCollision;

	NewSphereRadius = 415.f;

}

// Called when the game starts or when spawned
void ARoomTarget::BeginPlay()
{
	Super::BeginPlay();
	
}

void ARoomTarget::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	SphereCollision->SetSphereRadius(NewSphereRadius);
}

// Called every frame
void ARoomTarget::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

