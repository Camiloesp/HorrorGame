// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/FlickeringLight.h"
#include "Components/PointLightComponent.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AFlickeringLight::AFlickeringLight()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComp"));
	SetRootComponent(RootComp);

	PointLight = CreateDefaultSubobject<UPointLightComponent>(TEXT("PointLight"));
	PointLight->SetupAttachment(RootComponent);

	Intensity = 5'000.f;
	MinFlickerIntensity = 3'500.f;
	MinTimeIntensityTimer = 0.05;
	MaxTimeIntensityTimer = 0.15;
}

// Called when the game starts or when spawned
void AFlickeringLight::BeginPlay()
{
	Super::BeginPlay();
	
	SetLightIntensity();
}

// Called every frame
void AFlickeringLight::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AFlickeringLight::SetLightIntensity()
{
	// Sets new light intensity
	float NewIntensity = (PointLight->Intensity > MinFlickerIntensity) ? MinFlickerIntensity : Intensity;
	PointLight->SetIntensity(NewIntensity);

	// Timer to call this function again, but with a new random float.
	FTimerHandle FlickeringTime;
	float RandomDelay = UKismetMathLibrary::RandomFloatInRange(MinTimeIntensityTimer, MaxTimeIntensityTimer);
	GetWorld()->GetTimerManager().SetTimer(FlickeringTime, this, &AFlickeringLight::SetLightIntensity, RandomDelay, false);
}