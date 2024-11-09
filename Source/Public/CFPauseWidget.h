// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CFBaseWidget.h"
#include "CFPauseWidget.generated.h"

class UButton;

UCLASS()
class CF_API UCFPauseWidget : public UCFBaseWidget
{
	GENERATED_BODY()
	
public:
	

protected:
	UPROPERTY(meta = (BindWidget))
		UButton* ClearPauseButton;

	virtual void NativeOnInitialized() override;

private:
	UFUNCTION()
		void OnClearPause();
};
