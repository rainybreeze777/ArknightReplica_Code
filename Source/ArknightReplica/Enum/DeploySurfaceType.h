// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DeploySurfaceType.generated.h"

UENUM(BlueprintType, meta = (Bitflags, UseEnumValuesAsMaskValuesInEditor = "true"))
enum class EDeploySurfaceType : uint8
{
	NONE		= 0 			UMETA(DisplayName = "None"),
	MELEE		= 1 << 0	UMETA(DisplayName = "Melee"),
	RANGE		= 1 << 1	UMETA(DisplayName = "Range")
};
ENUM_CLASS_FLAGS(EDeploySurfaceType);
