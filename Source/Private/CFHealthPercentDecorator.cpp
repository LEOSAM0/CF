// Fill out your copyright notice in the Description page of Project Settings.


#include "CFHealthPercentDecorator.h"
#include "AIController.h"
#include "CFUtils.h"
#include "CFHealthComponent.h"

UCFHealthPercentDecorator::UCFHealthPercentDecorator()
{
	NodeName = "Health Percent";
}

bool UCFHealthPercentDecorator::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	const auto Controller = OwnerComp.GetAIOwner();
	if (!Controller) return false;

	const auto HealthComponent = CFUtils::GetCFPlayerComponent<UCFHealthComponent>(Controller->GetPawn());
	if (!HealthComponent || HealthComponent->IsDead()) return false;

	return HealthComponent->GetHealthPercent() <= HealthPercent;
}