// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CFBaseCharacter.h"
#include "GameFramework/Character.h"
#include "CFAICharacter.generated.h"

class UBehaviorTree;
class UWidgetComponent;

UCLASS()
class CF_API ACFAICharacter : public ACFBaseCharacter
{
	GENERATED_BODY()
	
public:
		ACFAICharacter(const FObjectInitializer& ObjInit);

		UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI")
			UBehaviorTree* BehaviorTreeAsset;

		virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
		UWidgetComponent* HealthWidgetComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI")
		float HealthVisibilityDistance = 3000.f;

	virtual void BeginPlay() override;
	virtual void OnHealthChanged(float Health, float HealthDelta) override;
	virtual void OnDeath() override;

private:
	void UpdateHealthWidgetVisibility();
};
