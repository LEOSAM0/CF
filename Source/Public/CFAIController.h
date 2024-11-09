// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "CFAIController.generated.h"

class UCFAIPerceptionComponent;
class UCFRespawnComponent;

UCLASS()
class CF_API ACFAIController : public AAIController
{
	GENERATED_BODY()
public:
	ACFAIController();

	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
		UCFAIPerceptionComponent* CFAIPerceptionComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
		UCFRespawnComponent* RespawnComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
		FName FocusOnKeyName = "EnemyActor";

	virtual void OnPossess(APawn* InPawn) override;

	virtual void Tick(float DeltaTime) override;

private:
	AActor* GetFocusOnActor() const;
};
