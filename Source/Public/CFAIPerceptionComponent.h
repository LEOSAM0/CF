// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Perception/AIPerceptionComponent.h"
#include "CFAIPerceptionComponent.generated.h"

/**
 * 
 */
UCLASS()
class CF_API UCFAIPerceptionComponent : public UAIPerceptionComponent
{
	GENERATED_BODY()
	
public:
	AActor* GetClosestEnemy() const;
};
