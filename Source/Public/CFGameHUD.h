// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "CFCoreTypes.h"
#include "CFGameHUD.generated.h"

class UCFBaseWidget;

UCLASS()
class CF_API ACFGameHUD : public AHUD
{
	GENERATED_BODY()
	
public:
	virtual void DrawHUD() override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
		TSubclassOf<UUserWidget> PlayerHUDWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
		TSubclassOf<UUserWidget> PauseWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
		TSubclassOf<UUserWidget> GameOverWidgetClass;

	virtual void BeginPlay() override;

private:
	UPROPERTY()
		TMap<ECFMatchState, UCFBaseWidget*> GameWidgets;

	UPROPERTY()
		UCFBaseWidget* CurrentWidget = nullptr;

	void DrawCrossHair();
	void OnMatchStateChanged(ECFMatchState State);
};
