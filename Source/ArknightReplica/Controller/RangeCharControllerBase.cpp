// Fill out your copyright notice in the Description page of Project Settings.


#include "RangeCharControllerBase.h"

void ARangeCharControllerBase::BeginPlay()
{
  Super::BeginPlay();
  RunBehaviorTree(AIBehaviorTree);
}