// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DeployCostWidget.generated.h"

class UTextBlock;

/**
 * 
 */
UCLASS()
class ARKNIGHTREPLICA_API UDeployCostWidget : public UUserWidget
{
  GENERATED_BODY()

public:
  void NativeConstruct() override;

protected:
  UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
  UTextBlock* CostTextBlock;

public:
  UFUNCTION(BlueprintCallable)
  void SetCost(int32 Cost);
};
