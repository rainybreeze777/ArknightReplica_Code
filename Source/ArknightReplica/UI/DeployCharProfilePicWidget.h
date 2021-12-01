// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Layout/Geometry.h"
#include "DeployCharProfilePicWidget.generated.h"

class ADeployableCharacter;
class ACharacterDeployManager;
class UImage;
struct FEventReply;

/**
 * 
 */
UCLASS()
class ARKNIGHTREPLICA_API UDeployCharProfilePicWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void NativeConstruct() override;

public:
	UPROPERTY(Transient, BlueprintReadWrite, meta=(BindWidget))
	UImage* OperatorProfileImage;

protected:
	UPROPERTY(Transient, BlueprintReadOnly)
	TSubclassOf<ADeployableCharacter> DeployChar;

	UPROPERTY(Transient, BlueprintReadOnly)
	ACharacterDeployManager* DeployManager;

	UPROPERTY(Transient, BlueprintReadWrite)
	bool bIsLeftMouseButtonPressed;

public:
	UFUNCTION(BlueprintCallable)
	void SetDeployChar(TSubclassOf<ADeployableCharacter> InDeployChar);

	UFUNCTION(BlueprintCallable)
	FEventReply OnMouseButtonDownHandler(FGeometry MyGeometry, const FPointerEvent& MouseEvent);
};
