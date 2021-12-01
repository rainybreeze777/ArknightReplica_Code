// Fill out your copyright notice in the Description page of Project Settings.

#include "DeployCostWidget.h"
#include "Components/TextBlock.h"

void UDeployCostWidget::NativeConstruct()
{
  Super::NativeConstruct();
}

void UDeployCostWidget::SetCost(int32 Cost)
{
  FText Text = Cost >= 0 && Cost < 10 ? FText::FromString("0" + FString::FromInt(Cost)) : FText::AsNumber(Cost);
  CostTextBlock->SetText(Text);
}