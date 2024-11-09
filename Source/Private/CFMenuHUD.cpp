// Fill out your copyright notice in the Description page of Project Settings.


#include "CFMenuHUD.h"
#include "CFBaseWidget.h"

void ACFMenuHUD::BeginPlay()
{
	Super::BeginPlay();

	if (MenuWidgetClass)
	{
		const auto MenuWidget = CreateWidget<UCFBaseWidget>(GetWorld(), MenuWidgetClass);
		if (MenuWidget)
		{
			MenuWidget->AddToViewport();
			MenuWidget->Show();
		}
	}
}
