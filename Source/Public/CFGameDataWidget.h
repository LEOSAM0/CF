// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CFGameDataWidget.generated.h"

class ACFGameModeBase;
class ACF_GMB_OnePlayer;
class ACFPlayerState;

UCLASS()
class CF_API UCFGameDataWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	

	UFUNCTION(BlueprintCallable, Category = "UI")
	int32 GetCurrentRoundNum() const;

	UFUNCTION(BlueprintCallable, Category = "UI")
		int32 GetCurrentRoundNumPlayer() const;

	UFUNCTION(BlueprintCallable, Category = "UI")
	int32 GetTotalRoundsNum() const;

	UFUNCTION(BlueprintCallable, Category = "UI")
		int32 GetTotalRoundsNumPlayer() const;

	UFUNCTION(BlueprintCallable, Category = "UI")
	int32 GetRoundSecondsRemaining() const;

	UFUNCTION(BlueprintCallable, Category = "UI")
		int32 GetRoundSecondsRemainingPlayer() const;

private:
	ACFGameModeBase* GetCFGameMode() const;
	ACF_GMB_OnePlayer* GetCFGameModePlayer() const;
	ACFPlayerState* GetCFPlayerState() const;

};
