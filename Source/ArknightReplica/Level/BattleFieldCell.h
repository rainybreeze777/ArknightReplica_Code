// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/StaticMeshActor.h"
#include "../Enum/DeploySurfaceType.h"
#include "BattleFieldCell.generated.h"

class UStaticMeshComponent;

/**
 * 
 */
UCLASS()
class ARKNIGHTREPLICA_API ABattleFieldCell : public AStaticMeshActor
{
	GENERATED_BODY()

public:
	ABattleFieldCell();

protected:
	UPROPERTY(VisibleInstanceOnly, Category = "Battlefield Cell")
	FVector OriginalScale;

	UPROPERTY(BlueprintReadWrite, EditInstanceOnly, Category = "Battlefield Cell", meta = (Bitmask, BitmaskEnum = EDeploySurfaceType))
	int32 AllowedDeployTypes;

public:
	virtual void PostActorCreated() override;
	
	/**
	 * Gets the surface plane before any edits to height of this cell have been
	 * made.
	 */
	UFUNCTION(BlueprintPure)
	FPlane GetOriginalHeightSurfacePlane() const;

	/**
	 * Gets surface plane of this cell at its current height.
	 */
	UFUNCTION(BlueprintPure)
	FPlane GetSurfacePlane() const;

	/**
	 * Gets the center point of this cell's surface
	 */
	UFUNCTION(BlueprintPure)
	FVector GetSurfaceCenter() const;

	/**
	 * Gets an enum flag EDeploySurfaceType indicating types of characters that 
	 * can be deployed on the surface of this battlefield cell.
	 */
	UFUNCTION(BlueprintPure)
	FORCEINLINE int32 GetAllowedDeployTypes() const { return AllowedDeployTypes; }

	UFUNCTION(BlueprintPure)
	FORCEINLINE bool IsWithinCell(const FVector &Point) const { return GetComponentsBoundingBox().IsInsideXY(Point); }

protected:
	virtual void BeginPlay() override;

#if WITH_EDITORONLY_DATA
public:
	UPROPERTY(Transient, EditInstanceOnly, Category = "Battlefield Cell")
	float SetCellHeightTo;
#endif

#if WITH_EDITOR
public:
	bool CanDeleteSelectedActor(FText& OutReason) const override;

	UFUNCTION(CallInEditor, Category = "Battlefield Cell")
	void SetHeight();
#endif
};
