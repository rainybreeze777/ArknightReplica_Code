// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HitVFXData.generated.h"

USTRUCT(BlueprintType)
struct FHitVFXData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	FVector HitLocation;

	UPROPERTY(BlueprintReadWrite)
	FVector HitOrigin;

	UPROPERTY(BlueprintReadWrite)
	FVector HitDirection;
};
