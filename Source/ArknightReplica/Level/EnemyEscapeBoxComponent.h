// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "EnemyEscapeBoxComponent.generated.h"

class ALevelGameStateBase;

/**
 * 
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class ARKNIGHTREPLICA_API UEnemyEscapeBoxComponent : public UBoxComponent
{
	GENERATED_BODY()

protected:
	UPROPERTY(Transient, BlueprintReadOnly)
	ALevelGameStateBase* GameState;

protected:
	void BeginPlay() override;

private:
	UFUNCTION()
	void OnComponentBeginOverlapDel(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
