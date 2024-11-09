// Fill out your copyright notice in the Description page of Project Settings.


#include "CFBaseWidget.h"
#include "sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"

void UCFBaseWidget::Show()
{
	PlayAnimation(ShowAnimation);
	UGameplayStatics::PlaySound2D(GetWorld(), OpenSound);
}