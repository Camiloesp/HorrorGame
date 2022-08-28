// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/JumpscareActors/JumpscareMaster.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"

#include "Sound/SoundCue.h"
#include "GameStates/L1GameState.h"

// Sets default values
AJumpscareMaster::AJumpscareMaster()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));
	SetRootComponent(RootComp);

	DetectionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Detection Box"));
	DetectionBox->SetupAttachment(RootComp);

	BoxPosition = FVector(0.f, 0.f, 0.f);
	BoxExtent = FVector(32.f, 32.f, 32.f);
}

// Called when the game starts or when spawned
void AJumpscareMaster::BeginPlay()
{
	Super::BeginPlay();
	
	DetectionBox->OnComponentBeginOverlap.AddDynamic(this, &AJumpscareMaster::OnOverlapBegin);
	DetectionBox->OnComponentEndOverlap.AddDynamic(this, &AJumpscareMaster::OnOverlapEnd);

	AL1GameState* GameState = Cast<AL1GameState>(GetWorld()->GetGameState());
	if (GameState)
	{
		FObjectiveData CurrentObjective = GameState->FindCurrentObjective();
		QuestID = CurrentObjective.IDName;
	}
}

void AJumpscareMaster::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	//Edit components post variables initialization
	DetectionBox->SetBoxExtent(BoxExtent);
	DetectionBox->SetRelativeLocation(BoxPosition);
}

// Called every frame
void AJumpscareMaster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AJumpscareMaster::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	TriggerJumpscare(OtherActor);
}

void AJumpscareMaster::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}

void AJumpscareMaster::TriggerJumpscare(AActor* OverlappingActor)
{
	if (!JumpscareSound) return;

	UGameplayStatics::PlaySound2D(GetWorld(), JumpscareSound);
}

