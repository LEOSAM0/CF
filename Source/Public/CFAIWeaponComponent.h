// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CFWeaponComponent.h"
#include "CFAIWeaponComponent.generated.h"

/**
 * 
 */
UCLASS()
class CF_API UCFAIWeaponComponent : public UCFWeaponComponent
{
	GENERATED_BODY()
	
public:
	virtual void StartFire() override;
	virtual void NextWeapon() override;
};
