// Fill out your copyright notice in the Description page of Project Settings.


#include "CFRespawnComponent.h"
#include "CF/CFGameModeBase.h"

UCFRespawnComponent::UCFRespawnComponent()
{
	
	PrimaryComponentTick.bCanEverTick = false;

	
}

void UCFRespawnComponent::Respawn(int32 RespawnTime)
{
	if (!GetWorld()) return;

	RespawnCountDown = RespawnTime;
	GetWorld()->GetTimerManager().SetTimer(RespawnTimerHandle, this, &UCFRespawnComponent::RespawnTimerUpdate, 1.f, true);
}

void UCFRespawnComponent::RespawnTimerUpdate()
{
	if (--RespawnCountDown == 0)
	{
		if (!GetWorld()) return;
		GetWorld()->GetTimerManager().ClearTimer(RespawnTimerHandle);

		const auto GameMode = Cast<ACFGameModeBase>(GetWorld()->GetAuthGameMode());
		if (!GameMode) return;

		GameMode->RespawnRequest(Cast<AController>(GetOwner()));
	}
}

bool UCFRespawnComponent::IsRespawnInProgress() const
{
	return GetWorld() && GetWorld()->GetTimerManager().IsTimerActive(RespawnTimerHandle);
}