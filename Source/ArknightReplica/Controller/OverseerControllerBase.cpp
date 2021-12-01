// Fill out your copyright notice in the Description page of Project Settings.


#include "OverseerControllerBase.h"
#include "../Level/BattleFieldCell.h"

AOverseerControllerBase::AOverseerControllerBase()
	: bIsLeftMouseButtonHolding(false)
{
	PrimaryActorTick.bCanEverTick = true;
	bEnableClickEvents = 1;
}

void AOverseerControllerBase::BeginPlay()
{
	Super::BeginPlay();
	SetShowMouseCursor(true);
	BattleFieldCellCollisionObjTypes.Add(UEngineTypes::ConvertToObjectType(ECC_BattlefieldCell));
}

void AOverseerControllerBase::Tick(float DeltaTime)
{
}

ABattleFieldCell* AOverseerControllerBase::GetCursorPointingCell() const
{
	FHitResult HitObject;
	if (GetHitResultUnderCursorForObjects(BattleFieldCellCollisionObjTypes, false, HitObject))
	{
		return Cast<ABattleFieldCell>(HitObject.Actor.Get());
	}
	return nullptr;
}

bool AOverseerControllerBase::GetCursorPointOnPlane(FPlane InPlane, FVector& IntersectPoint) const
{
	FVector CursorWorldLocation;
	FVector CursorWorldDirection;
	if (!DeprojectMousePositionToWorld(CursorWorldLocation, CursorWorldDirection))
	{
		return false;
	}
	if (FMath::IsNearlyZero(CursorWorldDirection | InPlane.GetNormal()))
	{
		return false;
	}
	IntersectPoint = FMath::RayPlaneIntersection(CursorWorldLocation, CursorWorldDirection, InPlane);
	return true;
}
