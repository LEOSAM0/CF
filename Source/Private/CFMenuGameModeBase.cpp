// Fill out your copyright notice in the Description page of Project Settings.


#include "CFMenuGameModeBase.h"
#include "CFMenuPlayerController.h"
#include "CFMenuHUD.h"

ACFMenuGameModeBase::ACFMenuGameModeBase()
{
	PlayerControllerClass = ACFMenuPlayerController::StaticClass();
	HUDClass = ACFMenuHUD::StaticClass();
}