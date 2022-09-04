// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Classic/AI_Basic.h"
#include "Animation/AnimationAsset.h"

#include "AI/Classic/Basic_AIController.h"
#include "Characters/HGCharacter.h"

#include "Blueprint/UserWidget.h"
#include "Widgets/GameOverWidget.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

// Sets default values
AAI_Basic::AAI_Basic()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AAI_Basic::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAI_Basic::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AAI_Basic::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AAI_Basic::CreateGameOverWidget()
{
	ABasic_AIController* ControllerAI = Cast<ABasic_AIController>(GetController());
	if (!ControllerAI || !ControllerAI->GetPlayerTarget()) return;

	//GetGameOverWidgetClass
	AHGCharacter* PlayerTarget = ControllerAI->GetPlayerTarget();

	UGameOverWidget* PlayerGameOverWidget = CreateWidget<UGameOverWidget>(GetWorld(), PlayerTarget->GameOverWidgetClass);
	if (!PlayerGameOverWidget) return;

	PlayerGameOverWidget->AddToViewport();

	APlayerController* PlayerTargetController = Cast<APlayerController>(PlayerTarget->GetController());
	if (!PlayerTargetController) return;

	PlayerTargetController->SetShowMouseCursor(true);
	UWidgetBlueprintLibrary::SetInputMode_UIOnlyEx(PlayerTargetController, PlayerGameOverWidget);
}

void AAI_Basic::KillPlayer()
{
	if (!AttackAnimation) return;

	float MontageDuration = AttackAnimation->GetPlayLength();
	GetMesh()->GetAnimInstance()->Montage_Play(AttackAnimation);

	FTimerHandle GameOverHandle;
	GetWorld()->GetTimerManager().SetTimer(GameOverHandle, this, &AAI_Basic::CreateGameOverWidget, MontageDuration+0.5f, false);
}

