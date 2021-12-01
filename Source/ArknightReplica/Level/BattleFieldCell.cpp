// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleFieldCell.h"
#include "Math/UnrealMathUtility.h"

ABattleFieldCell::ABattleFieldCell()
{
#if WITH_EDITORONLY_DATA
	bLockLocation = 1;
#endif
}

void ABattleFieldCell::PostActorCreated()
{
	AStaticMeshActor::PostActorCreated();
	OriginalScale = GetActorScale();
}

void ABattleFieldCell::BeginPlay()
{
}

FPlane ABattleFieldCell::GetOriginalHeightSurfacePlane() const
{
	float OriginalHeight = GetComponentsBoundingBox().Max.Z / GetActorScale().Z * OriginalScale.Z;
	return FPlane(GetActorLocation() + FVector(0, 0, OriginalHeight), FVector::UpVector);
}

FPlane ABattleFieldCell::GetSurfacePlane() const
{
	return FPlane(GetSurfaceCenter(), FVector::UpVector);
}

FVector ABattleFieldCell::GetSurfaceCenter() const
{
	return GetActorLocation() + FVector(0, 0, GetComponentsBoundingBox().Max.Z);
}

#if WITH_EDITOR
bool ABattleFieldCell::CanDeleteSelectedActor(FText& OutReason) const
{
	FString Msg(TEXT("BattleFieldCell should not be directly deleted from editor! Instead, regenerate the whole BattleField from BattleFieldGeneratorComponent."));
	OutReason = FText::FromString(Msg);
	return true;
}

void ABattleFieldCell::SetHeight()
{
	SetCellHeightTo = FMath::Floor(SetCellHeightTo * 4 + 0.5) / 4;
	SetActorScale3D(FVector(OriginalScale.X, OriginalScale.Y, OriginalScale.Z * SetCellHeightTo));
}
#endif