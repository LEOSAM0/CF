// Fill out your copyright notice in the Description page of Project Settings.


#include "CFPlayerController.h"
#include "CFRespawnComponent.h"
#include "CF/CFGameModeBase.h"
#include "CFGameInstance.h"
#include "CF_GMB_OnePlayer.h"

ACFPlayerController::ACFPlayerController()
{
	RespawnComponent = CreateDefaultSubobject<UCFRespawnComponent>("RespawnComponent");
}

void ACFPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (GetWorld())
	{
		if (const auto GameMode = Cast<ACFGameModeBase>(GetWorld()->GetAuthGameMode()))
		{
			GameMode->OnMatchStateChanged.AddUObject(this, &ACFPlayerController::OnMatchStateChanged);
		}
	}

	if (GetWorld())
	{
		if (const auto GameMode = Cast<ACF_GMB_OnePlayer>(GetWorld()->GetAuthGameMode()))
		{
			GameMode->OnMatchStateChanged.AddUObject(this, &ACFPlayerController::OnMatchStateChanged);
		}
	}
}

void ACFPlayerController::OnMatchStateChanged(ECFMatchState State)
{
	if (State == ECFMatchState::InProgress)
	{
		SetInputMode(FInputModeGameOnly());
		bShowMouseCursor = false;
	}
	else
	{
		SetInputMode(FInputModeUIOnly());
		bShowMouseCursor = true;
	}
}

void ACFPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	OnNewPawn.Broadcast(InPawn);
}

void ACFPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	if (!InputComponent) return;

	InputComponent->BindAction("PauseGame", IE_Pressed, this, &ACFPlayerController::OnPauseGame);
	InputComponent->BindAction("Mute", IE_Pressed, this, &ACFPlayerController::OnMuteSound);
}

void ACFPlayerController::OnPauseGame()
{
	if (!GetWorld() || !GetWorld()->GetAuthGameMode()) return;

	GetWorld()->GetAuthGameMode()->SetPause(this);
}

void ACFPlayerController::OnMuteSound()
{
	if (!GetWorld()) return;

	const auto CFGameInstance = GetWorld()->GetGameInstance<UCFGameInstance>();
	if (!CFGameInstance) return;

	CFGameInstance->ToggleVolume();
}