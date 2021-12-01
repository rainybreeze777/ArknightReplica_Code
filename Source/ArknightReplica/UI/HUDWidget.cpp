// Fill out your copyright notice in the Description page of Project Settings.

#include "HUDWidget.h"
#include "../Deployable/DeployableCharacter.h"
#include "DeployCharProfilePicWidget.h"
#include "Components/DynamicEntryBox.h"
#include "../PlayerState/DefaultPlayerStateBase.h"
#include "Kismet/GameplayStatics.h"

void UHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();

	ADefaultPlayerStateBase* PlayerState = Cast<ADefaultPlayerStateBase>(UGameplayStatics::GetActorOfClass(GetWorld(), ADefaultPlayerStateBase::StaticClass()));
	for (const TSubclassOf<ADeployableCharacter>& Char : PlayerState->AvailableOperators)
	{
		CreateOperatorProfilePic(Char);
	}
}

void UHUDWidget::CreateOperatorProfilePic(TSubclassOf<ADeployableCharacter> Operator)
{
	UDeployCharProfilePicWidget* Widget = OperatorsProfilePicContainer->CreateEntry<UDeployCharProfilePicWidget>();
	Widget->SetDeployChar(Operator);
}