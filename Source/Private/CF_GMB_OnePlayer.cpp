// Fill out your copyright notice in the Description page of Project Settings.


#include "CF_GMB_OnePlayer.h"
#include "CFPlayerController.h"
#include "CFBaseCharacter.h"
#include "CFGameHUD.h"
#include "AIController.h"
#include "CFPlayerStatePlayer.h"
#include "CFUtils.h"
#include "CFRespawnComponent.h"
#include "CFWeaponComponent.h"
#include "EngineUtils.h"
#include "CFGameInstance.h"

constexpr static int32 MinRoundTimeRespawn = 10;//in seconds

ACF_GMB_OnePlayer::ACF_GMB_OnePlayer()
{
	PlayerControllerClass = ACFPlayerController::StaticClass();
	DefaultPawnClass = ACFBaseCharacter::StaticClass();
	HUDClass = ACFGameHUD::StaticClass();
	PlayerStateClass = ACFPlayerStatePlayer::StaticClass();

	//auto Kills = Cast< ACFPlayerStatePlayer>(Kills->GetKillsNum());
}

void ACF_GMB_OnePlayer::StartPlay()
{
	Super::StartPlay();

	SpawnBots();
	//CreateTeamsInfo();

	CurrentRound = 1;
	StartRound();

	SetMatchState(ECFMatchState::InProgress);
}

UClass* ACF_GMB_OnePlayer::GetDefaultPawnClassForController_Implementation(AController* InController)
{
	if (InController && InController->IsA<AAIController>())
	{
		return AIPawnClass;
	}
	return Super::GetDefaultPawnClassForController_Implementation(InController);
}

void ACF_GMB_OnePlayer::SpawnBots()
{
	if (!GetWorld()) return;

	for (int32 i = 0; i < PlayersNum - 1; ++i)
	{
		FActorSpawnParameters SpawnInfo;
		SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		const auto CFAIController = GetWorld()->SpawnActor<AAIController>(AIControllerClass, SpawnInfo);
		RestartPlayer(CFAIController);
	}
}

void ACF_GMB_OnePlayer::StartRound()
{
	RoundCountDown = GameData.RoundTime;
	GetWorldTimerManager().SetTimer(GameRoundTimerHandle, this, &ACF_GMB_OnePlayer::GameTimerUpdate, 1.f, true);
}

void ACF_GMB_OnePlayer::GameTimerUpdate()
{
	//UE_LOG(LogCFGameModeBase, Display, TEXT("Time: %i / Round: %i/%i"), RoundCountDown, CurrentRound, GameData.RoundsNum);

	if (--RoundCountDown == 0)
	{
		GetWorldTimerManager().ClearTimer(GameRoundTimerHandle);

		if (CurrentRound + 1 <= GameData.RoundsNum)
		{
			++CurrentRound;
			ResetPlayers();
			StartRound();
		}
		else
		{
			GameOver();
		}
	}
}

void ACF_GMB_OnePlayer::ResetPlayers()
{
	if (!GetWorld()) return;

	for (auto It = GetWorld()->GetControllerIterator(); It; ++It)
	{
		ResetOnePlayer(It->Get());
	}
}

void ACF_GMB_OnePlayer::ResetOnePlayer(AController* Controller)
{
	if (Controller && Controller->GetPawn())
	{
		Controller->GetPawn()->Reset();
	}
	RestartPlayer(Controller);
	//SetPlayerColor(Controller);
}

void ACF_GMB_OnePlayer::CreateTeamsInfo()
{
	if (!GetWorld()) return;

	//int32 TeamID = 1;
	for (auto It = GetWorld()->GetControllerIterator(); It; ++It)
	{
		const auto Controller = It->Get();
		if (!Controller) continue;

		const auto PlayerState = Cast<ACFPlayerStatePlayer>(Controller->PlayerState);
		if (!PlayerState) continue;

		//PlayerState->SetTeamID(TeamID);
		//PlayerState->SetTeamColor(DetermineColorByTeamID(TeamID));
		PlayerState->GetPlayerName();// SetPlayerName(Controller->IsPlayerController() ? "Player" : "Bot");
		//SetPlayerColor(Controller);
		//TeamID = TeamID == 1 ? 2 : 1;
	}
}

void ACF_GMB_OnePlayer::Killed(AController* KillerController, AController* VictimController)
{
	/*const auto KillerPlayerState = KillerController ? Cast<ACFPlayerState>(KillerController->PlayerState) : nullptr;
	const auto VictimPlayerState = VictimController ? Cast<ACFPlayerState>(VictimController->PlayerState) : nullptr;

	if (KillerPlayerState)
	{
		KillerPlayerState->AddKill();
	}

	if (VictimPlayerState)
	{
		VictimPlayerState->AddDeath();
	}*/

	StartRespawn(VictimController);
}

void ACF_GMB_OnePlayer::LogPlayerInfo()
{
	if (!GetWorld()) return;

	for (auto It = GetWorld()->GetControllerIterator(); It; ++It)
	{
		const auto Controller = It->Get();
		if (!Controller) continue;

		const auto PlayerState = Cast<ACFPlayerStatePlayer>(Controller->PlayerState);
		if (!PlayerState) continue;

		//PlayerState->LogInfo();
	}
}

void ACF_GMB_OnePlayer::StartRespawn(AController* Controller)
{
	const auto RespawnAvailable = RoundCountDown > MinRoundTimeRespawn + GameData.RespawnTime;
	if (!RespawnAvailable) return;

	const auto RespawnComponent = CFUtils::GetCFPlayerComponent<UCFRespawnComponent>(Controller);
	if (!RespawnComponent) return;

	RespawnComponent->Respawn(RespawnTime);
}

void ACF_GMB_OnePlayer::RespawnRequest(AController* Controller)
{
	ResetOnePlayer(Controller);
}

void ACF_GMB_OnePlayer::GameOver()
{
	LogPlayerInfo();

	for (auto Pawn : TActorRange<APawn>(GetWorld()))
	{
		if (Pawn)
		{
			Pawn->TurnOff();
			Pawn->DisableInput(nullptr);
		}
	}
	SetMatchState(ECFMatchState::GameOver);
}

void ACF_GMB_OnePlayer::SetMatchState(ECFMatchState State)
{
	if (MatchState == State) return;

	MatchState = State;
	OnMatchStateChanged.Broadcast(MatchState);
}

bool ACF_GMB_OnePlayer::SetPause(APlayerController* PC, FCanUnpause CanUnpauseDelegate)
{
	const auto PauseSet = Super::SetPause(PC, CanUnpauseDelegate);

	if (PauseSet)
	{
		SetMatchState(ECFMatchState::Pause);
	}

	return PauseSet;
}

bool ACF_GMB_OnePlayer::ClearPause()
{
	const auto PauseCleared = Super::ClearPause();
	if (PauseCleared)
	{
		StopAllFire();
		SetMatchState(ECFMatchState::InProgress);
	}
	return PauseCleared;
}

void ACF_GMB_OnePlayer::StopAllFire()
{
	for (auto Pawn : TActorRange<APawn>(GetWorld()))
	{
		const auto WeaponComponent = CFUtils::GetCFPlayerComponent<UCFWeaponComponent>(Pawn);
		if (!WeaponComponent) continue;

		WeaponComponent->StopFire();
		WeaponComponent->Zoom(false);
	}
}