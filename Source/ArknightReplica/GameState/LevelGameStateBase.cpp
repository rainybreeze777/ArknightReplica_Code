// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelGameStateBase.h"
#include "../Level/WaveSpawner.h"
#include "../GameMode/DefenceGameModeBase.h"
#include "Kismet/GameplayStatics.h"

ALevelGameStateBase::ALevelGameStateBase()
	: CurrentEscapedCount(0)
	, TotalEnemyCount(0)
{
}

void ALevelGameStateBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	GameModeBase = Cast<ADefenceGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	LevelWaveSpawner = Cast<AWaveSpawner>(UGameplayStatics::GetActorOfClass(GetWorld(), AWaveSpawner::StaticClass()));
	if (GetWorld()->IsGameWorld())
	{
		TotalEnemyCount = LevelWaveSpawner->GetTotalCount();
	}
}

void ALevelGameStateBase::OnEnemyEscape()
{
	++CurrentEscapedCount;
	EnemyEscapedDelegate.Broadcast(CurrentEscapedCount);
	if (CurrentEscapedCount >= GameModeBase->GetLoseAtCount())
	{
#if UE_BUILD_DEVELOPMENT
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "Mission Failed");
#endif
	}
}

void ALevelGameStateBase::OnEnemyKilled(int32 KillCount)
{
	EnemyKilledDelegate.Broadcast(KillCount);
	if (KillCount >= TotalEnemyCount)
	{
#if UE_BUILD_DEVELOPMENT
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, "Mission Completed");
#endif
	}
}

int32 ALevelGameStateBase::GetCurrentEscapedCount() const
{
	return CurrentEscapedCount;
}

int32 ALevelGameStateBase::GetTotalEnemyCount() const
{
	return TotalEnemyCount;
}
