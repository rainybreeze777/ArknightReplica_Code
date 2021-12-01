// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "RangeCharControllerBase.generated.h"

/**
 * 
 */
UCLASS()
class ARKNIGHTREPLICA_API ARangeCharControllerBase : public AAIController
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UBehaviorTree* AIBehaviorTree;

protected:
	virtual void BeginPlay() override;
};
