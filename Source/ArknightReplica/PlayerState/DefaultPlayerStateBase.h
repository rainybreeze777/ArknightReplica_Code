// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "DefaultPlayerStateBase.generated.h"

class ALevelGameStateBase;
class ADeployableCharacter;

/**
 * 
 */
UCLASS()
class ARKNIGHTREPLICA_API ADefaultPlayerStateBase : public APlayerState
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<TSubclassOf<ADeployableCharacter>> AvailableOperators;

protected:
	void BeginPlay() override;

	UPROPERTY(Transient, BlueprintReadOnly)
	int32 KillCount;

	UPROPERTY(Transient, BlueprintReadOnly)
	ALevelGameStateBase* GameState;

public:
	UFUNCTION(BlueprintCallable)
	void IncrementKillCount();

};
