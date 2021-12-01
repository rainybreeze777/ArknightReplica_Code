// Fill out your copyright notice in the Description page of Project Settings.


#include "WaveSpawner.h"
#include "..\Enemy\EnemyBase.h"
#include "..\Controller\EnemyControllerBase.h"
#include "../Level/WaveEnemyAction.h"
#include "Engine/TargetPoint.h"
#include "Kismet/GameplayStatics.h"
#include "../Level/BattleField.h"

// Sets default values
AWaveSpawner::AWaveSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AWaveSpawner::BeginPlay()
{
	Super::BeginPlay();

	if (!IsValid(BattleField))
	{
		BattleField = Cast<ABattleField>(UGameplayStatics::GetActorOfClass(GetWorld(), ABattleField::StaticClass()));
	}

	SpawnTimerHandles.SetNum(EnemyWaves.Num());
	for (int i = 0; i < EnemyWaves.Num(); ++i)
	{
		FTimerHandle& Handle = SpawnTimerHandles[i];
		FTimerDelegate SpawnDel;
		FEnemyWave& Wave = EnemyWaves[i];
		SpawnDel.BindUObject(this, &AWaveSpawner::SpawnEnemies, &Wave.EnemyGroups, Wave.SpawnPoint);
		GetWorld()->GetTimerManager().SetTimer(Handle, SpawnDel, Wave.SpawnTimeInSeconds, false);
	}
}

// Called every frame
void AWaveSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

int32 AWaveSpawner::GetTotalCount() const
{
	int32 TotalCount = 0;

	for (const FEnemyWave& Wave : EnemyWaves)
	{
		for (const FEnemyGroup& Group : Wave.EnemyGroups)
		{
			TotalCount += Group.SpawnCount;
		}
	}

	return TotalCount;
}

void AWaveSpawner::SpawnEnemies(TArray<FEnemyGroup>* SpawningGroups, AActor* SpawnPoint)
{
	for (const FEnemyGroup& Group : *SpawningGroups)
	{
		FTimerDelegate EntryDel;
		FEnemyGroupEntryTracker* Tracker = new FEnemyGroupEntryTracker();
		Tracker->Group = &Group;
		Tracker->EnemyLeftToSpawn = Group.SpawnCount;
		EntryTrackers.Add(Tracker);
		EntryDel.BindUObject(this, &AWaveSpawner::DoEnemyEntry, Tracker, SpawnPoint);
		GetWorld()->GetTimerManager().SetTimer(Tracker->EntryTimerHandle, EntryDel, Group.EntryIntervalTime, true, 0);
	}
}

void AWaveSpawner::DoEnemyEntry(FEnemyGroupEntryTracker* EntryTracker, AActor* SpawnPoint)
{
	const FEnemyGroup& Group = *(EntryTracker->Group);

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	
	int32 ToSpawn = FMath::Min(EntryTracker->EnemyLeftToSpawn, Group.CountPerEntry);
	EntryTracker->EnemyLeftToSpawn -= ToSpawn;
	for (int i = 0; i < ToSpawn; ++i)
	{
		AEnemyBase* SpawnedEnemy = GetWorld()->SpawnActor<AEnemyBase>(Group.Enemy, SpawnPoint->GetTransform(), SpawnParams);
		SpawnedEnemy->SetBattleField(BattleField);
		SpawnedEnemy->AIControllerClass = Group.EnemyController;
		SpawnedEnemy->SpawnDefaultController();
		AEnemyControllerBase* EnemyControllerBase = Cast<AEnemyControllerBase>(SpawnedEnemy->Controller);
		EnemyControllerBase->ToDoActions.Append(Group.Actions);
		BattleField->NotifySpawnedEnemy(SpawnedEnemy);
	}

	if (EntryTracker->EnemyLeftToSpawn <= 0)
	{
		GetWorld()->GetTimerManager().ClearTimer(EntryTracker->EntryTimerHandle);
		EntryTracker->EntryTimerHandle.Invalidate();
		int32 TrackerIndex = EntryTrackers.Find(EntryTracker);
		check(TrackerIndex >= 0);
		delete EntryTrackers[TrackerIndex];
		EntryTrackers.RemoveAt(TrackerIndex);
	}
}
