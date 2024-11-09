// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CFGoToMenuWidget.generated.h"

class UButton;

UCLASS()
class CF_API UCFGoToMenuWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(meta = (BindWidget))
		UButton* GoToMenuButon;

	virtual void NativeOnInitialized() override;

private:
	UFUNCTION()
	void OnGoToMenu();
};
