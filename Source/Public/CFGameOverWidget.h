// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CFBaseWidget.h"
#include "CFCoreTypes.h"
#include "CFGameOverWidget.generated.h"

class UVerticalBox;
class UButton;

UCLASS()
class CF_API UCFGameOverWidget : public UCFBaseWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(meta = (BindWidget))
		UVerticalBox* PlayerStatBox;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
		TSubclassOf<UUserWidget> PlayerStatRowWidgetClass;

	UPROPERTY(meta = (BindWidget))
		UButton* ResetLevelButton;

	virtual void NativeOnInitialized() override;

private:
	void OnMatchStateChanged(ECFMatchState State);
	void UpdatePlayerStat();

	UFUNCTION()
		void OnResetLevel();
};
