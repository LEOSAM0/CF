// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CFBaseWidget.h"
#include "CFCoreTypes.h"
#include "CFMenuWidget.generated.h"

class UButton;
//class UHorizontalBox;
class UVerticalBox;
class UCFGameInstance;
class UCFLevelItemWidget;
class USoundCue;

UCLASS()
class CF_API UCFMenuWidget : public UCFBaseWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(meta = (BindWidget))
		UButton* StartGameButton;

	UPROPERTY(meta = (BindWidget))
		UButton* QuitGameButton;

	/*UPROPERTY(meta = (BindWidget))
		UHorizontalBox* LevelItemBox;*/

		UPROPERTY(meta = (BindWidget))
		UVerticalBox* VerticalItemBox;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
		TSubclassOf<UUserWidget> LevelItemWidgetClass;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
		UWidgetAnimation* HideAnimation;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sound")
		USoundCue* StartGameSound;

	virtual void NativeOnInitialized() override;
	virtual void OnAnimationFinished_Implementation(const UWidgetAnimation* Animation) override;

private:
	UPROPERTY()
		TArray<UCFLevelItemWidget*> LevelItemWidgets;

	UFUNCTION()
		void OnStartGame();

	UFUNCTION()
		void OnQuitGame();

	void InitLevelItems();
	void OnLevelSelected(const FLevelData& Data);
	UCFGameInstance* GetCFGameInstance() const;
};