// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "CFCoreTypes.h"
#include "CFGameInstance.generated.h"

class USoundClass;

UCLASS()
class CF_API UCFGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	FLevelData GetStartupLevel() const { return StartupLevel; }
	void SetStartupLevel(const FLevelData& Data) { StartupLevel = Data; }

	TArray<FLevelData> GetLevelsData() const { return LevelsData; }

	FName GetMenuLevelName() const { return MenuLevelName; }

	void ToggleVolume();
	
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Game", meta = (ToolTip = "Level names must be unique!"))
		TArray<FLevelData> LevelsData;

	UPROPERTY(EditDefaultsOnly, Category = "Game")
		FName MenuLevelName = NAME_None;

	UPROPERTY(EditDefaultsOnly, Category = "Sound")
		USoundClass* MasterSoundClass;

	private:
		FLevelData StartupLevel;
};
