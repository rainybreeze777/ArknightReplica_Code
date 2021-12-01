// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "LevelGameStateBase.generated.h"

class AWaveSpawner;
class ADefenceGameModeBase;

/**
 * 
 */
UCLASS()
class ARKNIGHTREPLICA_API ALevelGameStateBase : public AGameStateBase
{
	GENERATED_BODY()

public:
	ALevelGameStateBase();

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FEnemyEscapedDel, int32, TotalEscapedCount);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FEnemyKilledDel, int32, TotalKilledCount);

protected:
	UPROPERTY(BlueprintReadWrite)
	int32 CurrentEscapedCount;

	UPROPERTY(BlueprintReadWrite)
	int32 TotalEnemyCount;

	UPROPERTY(BlueprintAssignable)
	FEnemyEscapedDel EnemyEscapedDelegate;

	UPROPERTY(BlueprintAssignable)
	FEnemyKilledDel EnemyKilledDelegate;


private:
	AWaveSpawner* LevelWaveSpawner;
	const ADefenceGameModeBase* GameModeBase;

public:
	UFUNCTION(BlueprintCallable)
	void OnEnemyEscape();

	UFUNCTION(BlueprintCallable)
	void OnEnemyKilled(int32 KillCount);

	UFUNCTION(BlueprintPure)
	int32 GetCurrentEscapedCount() const;

	UFUNCTION(BlueprintPure)
	int32 GetTotalEnemyCount() const;

	void PostInitializeComponents() override;

};
