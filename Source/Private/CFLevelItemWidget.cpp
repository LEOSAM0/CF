// Fill out your copyright notice in the Description page of Project Settings.


#include "CFLevelItemWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"

void UCFLevelItemWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (LevelSelectButton)
	{
		LevelSelectButton->OnClicked.AddDynamic(this, &UCFLevelItemWidget::OnLevelItemClicked);
		LevelSelectButton->OnHovered.AddDynamic(this, &UCFLevelItemWidget::OnLevelHovered);
		LevelSelectButton->OnUnhovered.AddDynamic(this, &UCFLevelItemWidget::OnLevelUnhovered);
	}
}

void UCFLevelItemWidget::OnLevelItemClicked()
{
	OnLevelSelected.Broadcast(LevelData);
}

void UCFLevelItemWidget::SetLevelData(const FLevelData& Data)
{
	LevelData = Data;

	if (LevelNameTextBlock)
	{
		LevelNameTextBlock->SetText(FText::FromName(Data.LevelDisplayName));
	}

	if (LevelImage)
	{
		LevelImage->SetBrushFromTexture(Data.LevelThumb);
	}
}

void UCFLevelItemWidget::SetSelected(bool IsSelected)
{
	if (LevelImage)
	{
		LevelImage->SetColorAndOpacity(IsSelected ? FLinearColor::Red : FLinearColor::White);
	}
}

void UCFLevelItemWidget::OnLevelHovered()
{
	if (FrameImage)
	{
		FrameImage->SetVisibility(ESlateVisibility::Visible);
	}
}

void UCFLevelItemWidget::OnLevelUnhovered()
{
	if (FrameImage)
	{
		FrameImage->SetVisibility(ESlateVisibility::Hidden);
	}
}