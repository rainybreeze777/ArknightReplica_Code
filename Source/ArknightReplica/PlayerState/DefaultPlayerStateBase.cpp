// Fill out your copyright notice in the Description page of Project Settings.


#include "DefaultPlayerStateBase.h"
#include "../Deployable/DeployableCharacter.h"
#include "../GameState/LevelGameStateBase.h"
#include "../UI/HUDWidget.h"

void ADefaultPlayerStateBase::BeginPlay()
{
  Super::BeginPlay();
  GameState = GetWorld()->GetGameState<ALevelGameStateBase>();
}

void ADefaultPlayerStateBase::IncrementKillCount()
{
  ++KillCount;
  GameState->OnEnemyKilled(KillCount);
}