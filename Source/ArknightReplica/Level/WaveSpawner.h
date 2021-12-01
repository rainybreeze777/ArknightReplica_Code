// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WaveSpawner.generated.h"

class ABattleField;
class AEnemyBase;
class AEnemyControllerBase;
class ATargetPoint;
class UWaveEnemyAction;

USTRUCT(BlueprintType)
struct FEnemyGroup
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<AEnemyBase> Enemy;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<AEnemyControllerBase> EnemyController;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 SpawnCount;

	/** 
	 * To avoid spawning all enemies in a group at the same time and cause
	 * collision issues, indicates how many enemies should be spawned per entry 
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta=(ClampMin="1"))
	int32 CountPerEntry = 1;

	/** The time interval between each entry for one spawn group */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta=(ClampMin="0.01"))
	float EntryIntervalTime = 1.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Instanced)
	TArray<UWaveEnemyAction*> Actions;
};

struct FEnemyGroupEntryTracker
{
	const FEnemyGroup* Group;
	FTimerHandle EntryTimerHandle;
	int32 EnemyLeftToSpawn;
};

USTRUCT(BlueprintType)
struct FEnemyWave
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<FEnemyGroup> EnemyGroups;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float SpawnTimeInSeconds;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	AActor* SpawnPoint;
};

UCLASS()
class ARKNIGHTREPLICA_API AWaveSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWaveSpawner();

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<FEnemyWave> EnemyWaves;

	UPROPERTY(BlueprintReadWrite, EditInstanceOnly)
	ABattleField* BattleField;

private:
	TArray<FTimerHandle> SpawnTimerHandles;
	TArray<FEnemyGroupEntryTracker*> EntryTrackers;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintPure)
	int32 GetTotalCount() const;

protected:
	void SpawnEnemies(TArray<FEnemyGroup>* SpawningGroups, AActor* SpawnPoint);

private:
	void DoEnemyEntry(FEnemyGroupEntryTracker* EntryTracker, AActor* SpawnPoint);

};
