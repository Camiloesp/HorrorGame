// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/RE/RECamera.h"
#include "Camera/CameraComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Components/BoxComponent.h"
//#include "RECamera.h"
#include "Actors/RE/RECamera.h"
#include "Characters/HGCharacter.h"

// Sets default values
ARECamera::ARECamera()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	SceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root Comp"));
	SetRootComponent(SceneComp);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Follow Camera"));
	Camera->SetupAttachment(RootComponent);
	
	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collision"));
	BoxCollision->SetupAttachment(RootComponent);


	BoxExtent = FVector(32.f, 32.f, 32.f);
	BoxPosition = FVector(0.f, 0.f, 0.f);
	bExitingCameraView = false;
}

// Called when the game starts or when spawned
void ARECamera::BeginPlay()
{
	Super::BeginPlay();
	
	BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &ARECamera::OnOverlapBegin);
	BoxCollision->OnComponentEndOverlap.AddDynamic(this, &ARECamera::OnOverlapEnd);
}

// Called every frame
void ARECamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Look at player
	if (!OverlapingPlayer) return;
	FVector Start = GetActorLocation();
	FVector End = OverlapingPlayer->GetActorLocation();
	FRotator NewLookAtRotation = UKismetMathLibrary::FindLookAtRotation(Start, End);

	Camera->SetWorldRotation(NewLookAtRotation);
}

void ARECamera::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	BoxCollision->SetBoxExtent(BoxExtent);
	BoxCollision->SetRelativeLocation(BoxPosition);
}

void ARECamera::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	OverlapingPlayer = Cast<AHGCharacter>(OtherActor);
	if (!OverlapingPlayer) return;

	PlayerController = Cast<APlayerController>(OverlapingPlayer->GetController());
	if (!PlayerController) return;

	PlayerController->SetViewTargetWithBlend(this);
}

void ARECamera::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (!OverlapingPlayer || !PlayerController) return;

	/* Used in case camera boxes overlap each other */
	bool bIsPlayerUsingThisCamera = (PlayerController->GetViewTarget() == this);
	
	if (bExitingCameraView && bIsPlayerUsingThisCamera)
	{
		PlayerController->SetViewTargetWithBlend(OverlapingPlayer);
	}
}

