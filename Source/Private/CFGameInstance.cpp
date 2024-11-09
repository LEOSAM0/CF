// Fill out your copyright notice in the Description page of Project Settings.


#include "CFGameInstance.h"
#include "CFSoundFuncLib.h"

void UCFGameInstance::ToggleVolume()
{
	UCFSoundFuncLib::ToggleSoundClassVolume(MasterSoundClass);
}