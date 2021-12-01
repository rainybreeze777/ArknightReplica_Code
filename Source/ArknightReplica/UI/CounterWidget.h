// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CounterWidget.generated.h"

class UTextBlock;
class USpacer;

/**
 * 
 */
UCLASS()
class ARKNIGHTREPLICA_API UCounterWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void NativeConstruct() override;

protected:
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UTextBlock* LimitTextBlock;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UTextBlock* CounterTextBlock;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	USpacer* TextSpacer;

public:
	UFUNCTION(BlueprintCallable)
	void SetCounterLimit(int32 Limit);

	UFUNCTION(BlueprintCallable)
	void SetCounter(int32 Counter);

	UFUNCTION(BlueprintCallable, CallInEditor)
	void SetCounterLimitVisibility(ESlateVisibility InVisibility);

};
