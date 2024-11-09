// Fill out your copyright notice in the Description page of Project Settings.


#include "CFCharacterMovementComponent.h"
#include "CFBaseCharacter.h"

float UCFCharacterMovementComponent::GetMaxSpeed() const
{
	const float MaxSpeed = Super::GetMaxSpeed();
	const ACFBaseCharacter* Player = Cast<ACFBaseCharacter>(GetPawnOwner());

	return Player && Player->IsRunning() ? MaxSpeed * RunModifier : MaxSpeed;
}
