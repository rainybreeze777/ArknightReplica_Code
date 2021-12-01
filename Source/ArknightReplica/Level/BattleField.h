// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BattleField.generated.h"

class UBattleFieldGeneratorComponent;
class ABattleFieldCell;
class AEnemyBase;

USTRUCT()
struct FBattleFieldGridColumn
{
	GENERATED_BODY()

public:
	UPROPERTY()
	TArray<ABattleFieldCell*> Column;

	FORCEINLINE void SetNum(int32 NewNum, bool bAllowShrinking) { Column.SetNum(NewNum, bAllowShrinking); }

	FORCEINLINE int32 Num() const { return Column.Num(); }

	FORCEINLINE ABattleFieldCell*& operator[](int32 index) { return Column[index]; }

	FORCEINLINE const ABattleFieldCell*const & operator[](int32 index) const { return Column[index]; }

	FORCEINLINE void Empty(int32 Slack = 0) { Column.Empty(Slack); }
};

UCLASS()
class ARKNIGHTREPLICA_API ABattleField : public AActor
{
	GENERATED_BODY()

// Generator is an editor only component of battlefield that is used to
// assist with battle field generation
friend class UBattleFieldGeneratorComponent;

public:	
	// Sets default values for this actor's properties
	ABattleField();
	
	UPROPERTY(VisibleInstanceOnly)
	UBattleFieldGeneratorComponent* Generator;

protected:
	// Currently for the purpose of marking this field as UPROPERTY,
	// FBattleFieldColumn wrapper struct has been created to get around the error
	// where UE4 complains about UPROPERTY not supporting nested arrays.
	// It is not immediately clear what effects does an empty UPROPERTY macro
	// have, but many sources online have suggested that it's a good idea to mark
	// members and UObject with UPROPERTY.
	// A trade-off seems to be that VSCode debugger can no longer inspect
	// the inner values of BattleFieldGrid when the wrapper struct is in effect.
	// In the future, when the effects of UPROPERTY is clearer, consider
	// using nested arrays TArray<TArray<ABattleFieldCell*>>
	UPROPERTY()
	TArray<FBattleFieldGridColumn> BattleFieldGrid;

	UPROPERTY(Transient, BlueprintReadOnly, VisibleInstanceOnly)
	TArray<AEnemyBase*> AliveEnemies;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintPure)
	int32 GetRowCount() const;

	UFUNCTION(BlueprintPure)
	int32 GetColumnCount() const;

	UFUNCTION(BlueprintPure)
	const TArray<AEnemyBase*>& GetAliveEnemies() const;

	/**
	 * Gets the top plane of this battlefield when first generated, i.e. top
	 * of the battlefield that every ground actor is standing on. Returning
	 * plane ignores any subsequent edits to battlefield cell height, and only
	 * return the original ground plane.
	 */
	UFUNCTION(BlueprintPure)
	FPlane GetGroundPlane() const;

	/**
	 * Gets the provided battlefield cell's coordinate. Return true if cell is
	 * found, and false otherwise.
	 * In the output OutCoord, X denotes which row is this cell located (counting 
	 * from top to bottom), and Y denotes which column is this cell located
	 * (counting from left to right)
	 * Note: this is different from our typical Cartesian Coordinates, where X
	 * is the horizontal axis from left to right, and Y is the vertical axis
	 * going from bottom to top
	 */
	UFUNCTION(BlueprintPure)
	bool GetCoordinateFromCell(const ABattleFieldCell* InCell, FIntPoint& OutCoord) const;

	UFUNCTION(BlueprintCallable)
	void NotifySpawnedEnemy(AEnemyBase* NewEnemy);

	UFUNCTION(BlueprintCallable)
	void NotifyDespawnEnemy(AEnemyBase* DespawningEnemy);

	UFUNCTION(BlueprintPure)
	ABattleFieldCell* GetCellFromCoordinate(const FIntPoint& InCoord);

	FORCEINLINE FBattleFieldGridColumn& operator[](int32 index) { return BattleFieldGrid[index]; }
};
