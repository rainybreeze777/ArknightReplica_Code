// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EnemyControllerBase.generated.h"

class ATargetPoint;
class UBehaviorTree;
class UWaveEnemyAction;

/**
 * 
 */
UCLASS()
class ARKNIGHTREPLICA_API AEnemyControllerBase : public AAIController
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadOnly)
	TArray<UWaveEnemyAction*> ToDoActions;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UBehaviorTree* AIBehaviorTree;

protected:
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintCallable)
	UWaveEnemyAction* GetNextAction();
};
