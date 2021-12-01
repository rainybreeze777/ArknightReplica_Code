// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyControllerBase.h"
#include "../Level/WaveEnemyAction.h"
#include "Engine/TargetPoint.h"

void AEnemyControllerBase::BeginPlay()
{
	Super::BeginPlay();
	RunBehaviorTree(AIBehaviorTree);
}

UWaveEnemyAction* AEnemyControllerBase::GetNextAction()
{
  if (ToDoActions.Num() <= 0) { return nullptr; }
  UWaveEnemyAction* ReturnAction = ToDoActions[0];
  ToDoActions.RemoveAt(0);
  return ReturnAction;
}