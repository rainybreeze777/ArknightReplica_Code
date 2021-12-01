// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "WaveEnemyAction.generated.h"

UENUM(BlueprintType)
enum class EEnemyActionType : uint8
{
	UNKNOWN		UMETA(DisplayName = "Unknown"),
	MOVE			UMETA(DisplayName = "Move"),
	WAIT			UMETA(DisplayName = "Wait"),
};

/**
 * 
 */
UCLASS(BlueprintType, EditInlineNew, DefaultToInstanced)
class ARKNIGHTREPLICA_API UWaveEnemyAction : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	EEnemyActionType EnemyActionType;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	AActor* MoveTo;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float WaitFor = -1.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float WaitUntil = -1.0f;
};
