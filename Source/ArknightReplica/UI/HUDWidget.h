// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HUDWidget.generated.h"

class UDynamicEntryBox;
class UDeployCharProfilePicWidget;
class ADeployableCharacter;

/**
 * 
 */
UCLASS()
class ARKNIGHTREPLICA_API UHUDWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void NativeConstruct() override;

public:
  UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
  UDynamicEntryBox* OperatorsProfilePicContainer;

public:
  UFUNCTION(BlueprintCallable)
  void CreateOperatorProfilePic(TSubclassOf<ADeployableCharacter> Operator);
};
