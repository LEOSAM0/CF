// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CFCoreTypes.h"
#include "CFPlayerController.generated.h"

class UCFRespawnComponent;

UCLASS()
class CF_API ACFPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	ACFPlayerController();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
		UCFRespawnComponent* RespawnComponent;

	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override;
	virtual void SetupInputComponent() override;

private:
	void OnPauseGame();
	void OnMatchStateChanged(ECFMatchState State);
	void OnMuteSound();
};
