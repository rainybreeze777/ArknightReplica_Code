// Fill out your copyright notice in the Description page of Project Settings.


#include "CounterWidget.h"
#include "Components/TextBlock.h"
#include "Components/Spacer.h"

void UCounterWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UCounterWidget::SetCounterLimit(int32 Limit)
{
	LimitTextBlock->SetText(FText::AsNumber(Limit));
}

void UCounterWidget::SetCounter(int32 Counter)
{
	CounterTextBlock->SetText(FText::AsNumber(Counter));
}

void UCounterWidget::SetCounterLimitVisibility(ESlateVisibility InVisibility)
{
	LimitTextBlock->SetVisibility(InVisibility);
	TextSpacer->SetVisibility(InVisibility);
}
