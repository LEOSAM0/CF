// Fill out your copyright notice in the Description page of Project Settings.


#include "CFNeedAmmoDecorator.h"
#include "AIController.h"
#include "CFUtils.h"
#include "CFWeaponComponent.h"

UCFNeedAmmoDecorator::UCFNeedAmmoDecorator()
{
	NodeName = "Need Ammo";
}

bool UCFNeedAmmoDecorator::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	const auto Controller = OwnerComp.GetAIOwner();
	if (!Controller) return false;

	const auto WeaponComponent = CFUtils::GetCFPlayerComponent<UCFWeaponComponent>(Controller->GetPawn());
	if (!WeaponComponent) return false;

	return WeaponComponent->NeedAmmo(WeaponType);
}