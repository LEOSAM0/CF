// Fill out your copyright notice in the Description page of Project Settings.


#include "CFMenuPlayerController.h"
#include "CFGameInstance.h"

void ACFMenuPlayerController::BeginPlay()
{
	Super::BeginPlay();

	SetInputMode(FInputModeUIOnly());
	bShowMouseCursor = true;

}