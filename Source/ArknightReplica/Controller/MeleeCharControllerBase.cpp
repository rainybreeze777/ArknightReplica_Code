// Fill out your copyright notice in the Description page of Project Settings.


#include "MeleeCharControllerBase.h"

void AMeleeCharControllerBase::BeginPlay()
{
  Super::BeginPlay();
  RunBehaviorTree(AIBehaviorTree);
}