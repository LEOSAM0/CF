// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "CFNeedAmmoDecorator.generated.h"

class ACFBaseWeapon;

UCLASS()
class CF_API UCFNeedAmmoDecorator : public UBTDecorator
{
	GENERATED_BODY()
	
public:
	UCFNeedAmmoDecorator();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
		TSubclassOf<ACFBaseWeapon> WeaponType;

	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};
