// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "CFHealthPercentDecorator.generated.h"

/**
 * 
 */
UCLASS()
class CF_API UCFHealthPercentDecorator : public UBTDecorator
{
	GENERATED_BODY()
	
public:
	UCFHealthPercentDecorator();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
		float HealthPercent = 0.6f;

	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};
