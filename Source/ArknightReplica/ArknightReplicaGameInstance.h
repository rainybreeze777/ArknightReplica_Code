// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "SharedStruct/LevelData.h"
#include "ArknightReplicaGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class ARKNIGHTREPLICA_API UArknightReplicaGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	FString LevelDataJsonPath;

private:
	TMap<FString, FLevelData> LevelDataMap;

public:
	UFUNCTION(BlueprintPure)
	int32 GetLossAtEscapedEnemyCount(const FString& LevelName) const;

	void Init() override;
};
