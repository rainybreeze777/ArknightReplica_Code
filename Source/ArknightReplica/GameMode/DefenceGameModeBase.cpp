// Fill out your copyright notice in the Description page of Project Settings.


#include "DefenceGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "../ArknightReplicaGameInstance.h"

void ADefenceGameModeBase::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
  Super::InitGame(MapName, Options, ErrorMessage);
  GameInstance = Cast<UArknightReplicaGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

  TArray<FString> Tokens;
  MapName.ParseIntoArray(Tokens, TEXT("_"));

  LoseAtEnemyEscapeCount = GameInstance->GetLossAtEscapedEnemyCount(Tokens[Tokens.Num() - 1]);
  if (LoseAtEnemyEscapeCount <= 0)
  {
    FString ErrMsg = FString::Printf(TEXT("Unable to retrieve loss condition for map %s"), *MapName);
    UE_LOG(LogAssetData, Error, TEXT("%s"), *ErrMsg);
    checkf(false, TEXT("%s"), *ErrMsg);
  }
}

int32 ADefenceGameModeBase::GetLoseAtCount() const
{
  return LoseAtEnemyEscapeCount;
}