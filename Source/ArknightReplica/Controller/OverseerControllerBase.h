// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "OverseerControllerBase.generated.h"

#define ECC_BattlefieldCell		ECC_GameTraceChannel1

class ABattleFieldCell;

/**
 * 
 */
UCLASS()
class ARKNIGHTREPLICA_API AOverseerControllerBase : public APlayerController
{
	GENERATED_BODY()

public:
	AOverseerControllerBase();

protected:
	UPROPERTY(BlueprintReadOnly)
	bool bIsLeftMouseButtonHolding;

private:
	TArray<TEnumAsByte<EObjectTypeQuery>> BattleFieldCellCollisionObjTypes;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintPure)
	ABattleFieldCell* GetCursorPointingCell() const;

	UFUNCTION(BlueprintPure)
	bool GetCursorPointOnPlane(FPlane InPlane, FVector &IntersectPoint) const;

	FORCEINLINE bool IsLeftMouseButtonUp() const { return !IsInputKeyDown(EKeys::LeftMouseButton); }

protected:
	void BeginPlay() override;
};
