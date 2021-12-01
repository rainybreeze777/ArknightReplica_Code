// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../ArknightReplicaGameModeBase.h"
#include "DefenceGameModeBase.generated.h"

class UArknightReplicaGameInstance;

/**
 * 
 */
UCLASS()
class ARKNIGHTREPLICA_API ADefenceGameModeBase : public AArknightReplicaGameModeBase
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(BlueprintReadOnly)
	int32 LoseAtEnemyEscapeCount;

private:
	UArknightReplicaGameInstance* GameInstance;

public:
	UFUNCTION(BlueprintPure)
	int32 GetLoseAtCount() const;

	void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
};
