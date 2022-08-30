// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/SecurityCameras/SecurityCamera.h"
#include "Camera/CameraComponent.h"

// Sets default values
ASecurityCamera::ASecurityCamera()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));
	SetRootComponent(SceneComp);

	SecurityCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Security Camera"));
	SecurityCamera->SetupAttachment(RootComponent);
	SecurityCamera->SetRelativeLocation(FVector(0.f, 0.f, 60.f));
}

// Called when the game starts or when spawned
void ASecurityCamera::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASecurityCamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

