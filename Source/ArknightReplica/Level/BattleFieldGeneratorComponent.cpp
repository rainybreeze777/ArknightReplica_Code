// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleFieldGeneratorComponent.h"
#include "BattleField.h"
#include "BattleFieldCell.h"
#include "Engine/StaticMeshActor.h"

// Sets default values for this component's properties
UBattleFieldGeneratorComponent::UBattleFieldGeneratorComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	bIsEditorOnly = true;
}


// Called when the game starts
void UBattleFieldGeneratorComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UBattleFieldGeneratorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UBattleFieldGeneratorComponent::SetBattleField(ABattleField* InBattleField)
{
	if (BattleField == nullptr)
	{
		BattleField = InBattleField;
	}
}

void UBattleFieldGeneratorComponent::GenerateBattleField()
{
	if (BattleField == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("BattleField of Generator Component cannot be null! did you forget to initialize it?"));
		return;
	}
	if (SetRowTo < 1 || SetColumnTo < 1)
	{
		UE_LOG(LogTemp, Error, TEXT("The number of battlefield rows and columns must be greater than 0!"));
		return;
	}

	ResetBattleField();

	FActorSpawnParameters Params;
	AStaticMeshActor* Spawned = GetWorld()->SpawnActor<AStaticMeshActor>(FieldCell, FTransform::Identity.GetScaled(CellScale), Params);
	FVector Origin;
	FVector HalfSize;
	Spawned->GetActorBounds(false, Origin, HalfSize, false);
	FVector TopLeftOrigin(-1 * HalfSize.X * (SetColumnTo - 1), -1 * HalfSize.X * (SetRowTo - 1), 0);
	BattleField->BattleFieldGrid.SetNum(SetRowTo, false);
	for (int r = 0; r < SetRowTo; ++r)
	{
		BattleField->BattleFieldGrid[r].SetNum(SetColumnTo, false);
		for (int c = 0; c < SetColumnTo; ++c)
		{
			FVector OriginShift(c * HalfSize.X * 2, r * HalfSize.X * 2, 0);
			FVector CellOrigin = TopLeftOrigin + OriginShift;
			FTransform SpawnLocation(FQuat::Identity, CellOrigin, FVector::OneVector * CellScale);
			if (Spawned != nullptr)
			{
				Spawned->SetActorTransform(SpawnLocation, false, nullptr, ETeleportType::TeleportPhysics);
			}
			else
			{
				Spawned = GetWorld()->SpawnActor<AStaticMeshActor>(FieldCell, SpawnLocation, Params);
			}
#if WITH_EDITOR
			Spawned->SetActorLabel(FString::Printf(TEXT("BattlefieldCell%d_%d"), r, c), true);
#endif
			Spawned->AttachToActor(BattleField, FAttachmentTransformRules::KeepWorldTransform);
			BattleField->BattleFieldGrid[r][c] = Cast<ABattleFieldCell>(Spawned);
			Spawned = nullptr;
		}
	}
}

void UBattleFieldGeneratorComponent::ResetBattleField()
{
	if (BattleField == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("BattleField of Generator Component cannot be null! did you forget to initialize it?"));
		return;
	}

	for (int r = 0; r < BattleField->BattleFieldGrid.Num(); ++r)
	{
		for (int c = 0; c < BattleField->BattleFieldGrid[r].Num(); ++c)
		{
			if (IsValid(BattleField->BattleFieldGrid[r][c]))
			{
				BattleField->BattleFieldGrid[r][c]->Destroy();
			}
		}
		BattleField->BattleFieldGrid[r].Empty();
	}
	BattleField->BattleFieldGrid.Empty();
}