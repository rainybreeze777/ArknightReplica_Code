// Fill out your copyright notice in the Description page of Project Settings.

#include "DeployCharProfilePicWidget.h"
#include "../Deployable/DeployableCharacter.h"
#include "../Level/CharacterDeployManager.h"
#include "Components/Image.h"
#include "Kismet/GameplayStatics.h"

void UDeployCharProfilePicWidget::NativeConstruct()
{
	Super::NativeConstruct();
	bIsLeftMouseButtonPressed = false;
	DeployManager = Cast<ACharacterDeployManager>(UGameplayStatics::GetActorOfClass(GetWorld(), ACharacterDeployManager::StaticClass()));
}

void UDeployCharProfilePicWidget::SetDeployChar(TSubclassOf<ADeployableCharacter> InDeployChar)
{
	DeployChar = InDeployChar;
	OperatorProfileImage->SetBrushFromTexture(InDeployChar.GetDefaultObject()->ProfilePic);
}

FEventReply UDeployCharProfilePicWidget::OnMouseButtonDownHandler(FGeometry MyGeometry, const FPointerEvent &MouseEvent)
{
	if (MouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton))
	{
		bIsLeftMouseButtonPressed = true;
// #if UE_BUILD_DEVELOPMENT
// 		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, "Left Mouse Down on Profile Pic");
// #endif
		DeployManager->InitiateDeployment(DeployChar);
		// Do NOT return FEventReply(true) here, as this will stop mouse click event
		// from bubbling up to player controller. Then Player Controller won't be
		// able to correctly handle mouse click events afterwards
	}
	return FEventReply(false);
}
