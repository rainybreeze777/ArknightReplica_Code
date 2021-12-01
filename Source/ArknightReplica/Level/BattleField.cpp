// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleField.h"
#include "BattleFieldGeneratorComponent.h"
#include "BattleFieldCell.h"
#include "NavMesh/NavMeshBoundsVolume.h"

// Sets default values
ABattleField::ABattleField()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootSceneComponent"));
	RootComponent->SetMobility(EComponentMobility::Static);

	SetActorLocation(FVector::ZeroVector);
	// Only initialize generator for editors only
#if WITH_EDITOR
	Generator = CreateDefaultSubobject<UBattleFieldGeneratorComponent>(TEXT("Battlefield Generator Component"));
	Generator->SetBattleField(this);
#endif

#if WITH_EDITORONLY_DATA
	bLockLocation = 1;
#endif
}

// Called when the game starts or when spawned
void ABattleField::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ABattleField::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

int32 ABattleField::GetRowCount() const
{
	return BattleFieldGrid.Num();
}

int32 ABattleField::GetColumnCount() const
{
	if (BattleFieldGrid.Num() == 0)
	{
		return 0;
	}

	return BattleFieldGrid[0].Num();
}

const TArray<AEnemyBase*>& ABattleField::GetAliveEnemies() const
{
	return AliveEnemies;
}

FPlane ABattleField::GetGroundPlane() const
{
	if (BattleFieldGrid.Num() == 0 || BattleFieldGrid[0].Num() == 0) { return FPlane(); }

	// Take the original surface of any battlefield cell, and use its Z as
	// height of this battlefield ground plane base point
	FPlane OriginalCellSurface = BattleFieldGrid[0][0]->GetOriginalHeightSurfacePlane();
	return FPlane(GetActorLocation() + FVector(0, 0, OriginalCellSurface.GetOrigin().Z), OriginalCellSurface.GetNormal());
}

bool ABattleField::GetCoordinateFromCell(const ABattleFieldCell* InCell, FIntPoint &OutCoord) const
{
	if (!IsValid(InCell))	{ return false; }
	for (int32 r = 0; r < BattleFieldGrid.Num(); ++r)
	{
		for (int32 c = 0; c < BattleFieldGrid[r].Num(); ++c)
		{
			if (InCell == BattleFieldGrid[r][c])
			{
				OutCoord.X = c;
				OutCoord.Y = r;
				return true;
			}
		}
	}
	return false;
}

void ABattleField::NotifySpawnedEnemy(AEnemyBase* NewEnemy)
{
	AliveEnemies.Add(NewEnemy);
}

void ABattleField::NotifyDespawnEnemy(AEnemyBase* DespawningEnemy)
{
	ensureAlwaysMsgf(AliveEnemies.Contains(DespawningEnemy), TEXT("Notifying the despawn of an enemy that doesn't exist!"));
	if (AliveEnemies.Contains(DespawningEnemy))
	{
		AliveEnemies.Remove(DespawningEnemy);
	}
}

ABattleFieldCell* ABattleField::GetCellFromCoordinate(const FIntPoint& InCoord)
{
	if (InCoord.Y < 0 || InCoord.Y >= GetRowCount() || InCoord.X < 0 || InCoord.X >= GetColumnCount())
	{
		return nullptr;
	}
	return BattleFieldGrid[InCoord.Y][InCoord.X];
}
