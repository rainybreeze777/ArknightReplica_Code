// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterDeployManager.h"
#include "Engine/World.h"
#include "BattleFieldCell.h"
#include "BattleField.h"
#include "../Controller/OverseerControllerBase.h"
#include "../Deployable/DeployableCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "DrawDebugHelpers.h"

// Sets default values
ACharacterDeployManager::ACharacterDeployManager()
	: DeployPhase(ECharacterDeployPhase::NONE)
	, DeploySelectPlaneHeight(350)
	, DeploySelectRadius(300)
	, CursorVector(FVector::ZeroVector)
	, ClickStartInRadius(EClickStartInRadius::NOT_STARTED)
	, bIsMouseHolding(false)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACharacterDeployManager::BeginPlay()
{
	Super::BeginPlay();
	if (!IsValid(BattleField))
	{
		BattleField = Cast<ABattleField>(UGameplayStatics::GetActorOfClass(GetWorld(), ABattleField::StaticClass()));
	}
	OverseerController = GetWorld()->GetFirstPlayerController<AOverseerControllerBase>();

	DeploySelectPlane = BattleField->GetGroundPlane();
	DeploySelectPlane =
		DeploySelectPlane.TransformBy(FTranslationMatrix::Make(FVector(0, 0, DeploySelectPlaneHeight)));

	SetActorEnableCollision(false);

	DeploymentPoint = StartingDeploymentPoint;

	FTimerDelegate DeploymentPointRecoverDel;
	DeploymentPointRecoverDel.BindLambda([this]()
		{ 
			ApplyDPChange(1);
		});
	GetWorldTimerManager().SetTimer(
			DeploymentPointRecoverHandle,
			DeploymentPointRecoverDel,
			1.0f,
			true);
}

// Called every frame
void ACharacterDeployManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	static ABattleFieldCell* DeployingToCell = nullptr;
	static ECharDeployOrientation DeployingOrientation = ECharDeployOrientation::NONE;
	bool bJustPressedLMB = false;
	bool bJustReleasedLMB = false;

	if (!bIsMouseHolding && !OverseerController->IsLeftMouseButtonUp())
	{
		bIsMouseHolding = true;
		bJustPressedLMB = true;
	}
	else if (bIsMouseHolding && OverseerController->IsLeftMouseButtonUp())
	{
		bIsMouseHolding = false;
		bJustReleasedLMB = true;
	}

	// If in focusing mode or deploy orientation mode, need to figure out the
	// vector on the deploy select plane from operator's center to cursor position
	if (DeployPhase != ECharacterDeployPhase::NONE && DeployPhase != ECharacterDeployPhase::POSITION)
	{
		ADeployableCharacter* TheChar = DeployPhase == ECharacterDeployPhase::FOCUSING ? FocusingOnChar : StagingChar;
		FVector CharPos = TheChar->GetActorLocation();
		FVector DeploySelectPlaneCenter = GetOrthoProjPosOnDeploySelectPlane(CharPos);
		FVector OnDeploySelectPlaneCursorPos;
		OverseerController->GetCursorPointOnPlane(DeploySelectPlane, OnDeploySelectPlaneCursorPos);
		CursorVector = OnDeploySelectPlaneCursorPos - DeploySelectPlaneCenter;
		// If pressed LMB, then need to remember if starting click is from within
		// deploy select valid circle
		if (bJustPressedLMB)
		{
			ClickStartInRadius = CursorVector.Size() >= DeploySelectRadius ? EClickStartInRadius::NO : EClickStartInRadius::YES;
		}

		if (DeployPhase == ECharacterDeployPhase::ORIENTATION)
		{
			bool bIsDragging = ClickStartInRadius == EClickStartInRadius::YES && bIsMouseHolding;
			UILogicUpdateCursorDrag(bIsDragging, DeploySelectPlaneCenter, CursorVector, DeploySelectRadius);
		}
	}

	if (bJustReleasedLMB)
	{
		// If previous tick is in position phase, and detects a LMB release, then
		// transition orientation phase if position phase returns a valid cell
		if (DeployPhase == ECharacterDeployPhase::POSITION)
		{
			if (IsValid(DeployingToCell))
			{
				FIntPoint CellCoord;
				verifyf(BattleField->GetCoordinateFromCell(DeployingToCell, CellCoord), TEXT("Failed to find coordinate for cell %s"), *DeployingToCell->GetName());
				StagingChar->CharLocation = CellCoord;
				DeployPhase = ECharacterDeployPhase::ORIENTATION;
				UILogicDeployEnterOrientationPhase();
			}
			else
			{
				StagingChar->Destroy();
				ResetDeployPhase();
			}
		}
		else if (DeployPhase == ECharacterDeployPhase::ORIENTATION)
		{
			if (DeployingOrientation != ECharDeployOrientation::NONE && ClickStartInRadius == EClickStartInRadius::YES)
			{
				StagingChar->CommitDeploy();
				ApplyDPChange(-1 * StagingChar->GetDeployCost());
				Unfocus();
			}
			else if (ClickStartInRadius == EClickStartInRadius::NO && CursorVector.Size() >= DeploySelectRadius)
			{
				StagingChar->Destroy();
				Unfocus();
			}
		}
		else if (DeployPhase == ECharacterDeployPhase::FOCUSING)
		{
			if (ClickStartInRadius == EClickStartInRadius::NO && CursorVector.Size() >= DeploySelectRadius)
			{
				Unfocus();
			}
		}
		ClickStartInRadius = EClickStartInRadius::NOT_STARTED;
	}

	switch (DeployPhase)
	{
		case ECharacterDeployPhase::POSITION:
			DeployingToCell = AdjustDeployPosition();
			break;
		case ECharacterDeployPhase::ORIENTATION:
		{
			auto PreviousOri = DeployingOrientation;
			DeployingOrientation = AdjustDeployOrientation(CursorVector, ClickStartInRadius, DeploySelectRadius);
			if (PreviousOri != DeployingOrientation)
			{
				UILogicUpdateDeployingOrientation(DeployingOrientation);
			}
			break;
		}
		default:
			break;
	}
}

void ACharacterDeployManager::InitiateDeployment(TSubclassOf<ADeployableCharacter> CharToDeploy)
{
	if (DeployPhase != ECharacterDeployPhase::NONE)
	{
		if (IsValid(StagingChar))
		{
			StagingChar->Destroy();
		}
		Unfocus();
	}
	DeployPhase = ECharacterDeployPhase::POSITION;
	bIsMouseHolding = true;
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	// Spawn to an arbitrary offscreen point if can't determine cursor point in world
	FVector CursorOnBattlefieldSurfacePoint(0, 0, 5000);
	OverseerController->GetCursorPointOnPlane(BattleField->GetGroundPlane(), CursorOnBattlefieldSurfacePoint);
	FVector DeployPoint = CharToDeploy.GetDefaultObject()->GetDeployPositionForPoint(CursorOnBattlefieldSurfacePoint);
	StagingChar = GetWorld()->SpawnActor<ADeployableCharacter>(CharToDeploy, DeployPoint, FRotator::ZeroRotator, SpawnParams);
	StagingChar->DeployManager = this;
}

ABattleFieldCell* ACharacterDeployManager::AdjustDeployPosition()
{
	ABattleFieldCell* Cell = OverseerController->GetCursorPointingCell();
	FVector OnCellSurfaceCursorPoint;
	FVector TeleportPoint;
	bool bTpPointFound = false;
	bool bIsValidDeploy = false;
	// First try to get where deploying character will stand on a battlefield cell
	if (Cell != nullptr && OverseerController->GetCursorPointOnPlane(Cell->GetSurfacePlane(), OnCellSurfaceCursorPoint))
	{
		bTpPointFound = true;
		if (Cell->GetAllowedDeployTypes() & StagingChar->GetDeployableSurfaceTypes())
		{
			bIsValidDeploy = true;
			TeleportPoint = StagingChar->GetDeployPositionForPoint(Cell->GetSurfaceCenter());
		}
		else
		{
			TeleportPoint = StagingChar->GetDeployPositionForPoint(OnCellSurfaceCursorPoint);
		}
	}
	// If a standing point isn't found yet, try let it stand on arbitrary
	// battlefield point instead near the cursor
	if (!bTpPointFound && OverseerController->GetCursorPointOnPlane(BattleField->GetGroundPlane(), OnCellSurfaceCursorPoint))
	{
		bTpPointFound = true;
		TeleportPoint = StagingChar->GetDeployPositionForPoint(OnCellSurfaceCursorPoint);
	}
	// Only move if a teleportation point is found
	if (bTpPointFound)
	{
		StagingChar->TeleportTo(TeleportPoint, FRotator::ZeroRotator, false, false);
	}

	return bIsValidDeploy ? Cell : nullptr;
}

ECharDeployOrientation ACharacterDeployManager::AdjustDeployOrientation(FVector InCursorVector, EClickStartInRadius InClickStartInRadius, float InSelectRadius)
{
	ECharDeployOrientation Orientation = ECharDeployOrientation::NONE;

	if (InClickStartInRadius == EClickStartInRadius::YES && InCursorVector.Size() >= InSelectRadius / 2)
	{
		FVector SouthEastVector = FVector(1, 1, 0).GetSafeNormal(0.0001f);
		// Find the angle between center to cursor vector, and south-east vector
		float CosineAngle = FVector::DotProduct(InCursorVector.GetSafeNormal(0.0001f), SouthEastVector);
		// The cross product of these two vectors will yield an orthogonal vector
		// that points downward if InCursorVector is within 180 degrees of SE vector
		// clockwise, or points upward if within 180 egrees of SE vector
		// counter-clock-wise
		bool bIsFacingSouthWest = FVector::CrossProduct(InCursorVector, SouthEastVector).Z < 0;

		// If we are within 90 degrees counter c-w or clock-wise of Southeast vector
		if (CosineAngle >= 0)
		{
			if (bIsFacingSouthWest)
			{
				Orientation = ECharDeployOrientation::SOUTH;
			}
			else
			{
				Orientation = ECharDeployOrientation::EAST;
			}
		}
		// Else we are between 90 ~ 180 degrees of Southeast vector
		else
		{
			if (bIsFacingSouthWest)
			{
				Orientation = ECharDeployOrientation::WEST;
			}
			else
			{
				Orientation = ECharDeployOrientation::NORTH;
			}
		}
		StagingChar->SetDeployOrientation(Orientation);
	}

	return Orientation;
}

void ACharacterDeployManager::ResetDeployPhase()
{
	StagingChar = nullptr;
	FocusingOnChar = nullptr;
	CursorVector = FVector::ZeroVector;
	DeployPhase = ECharacterDeployPhase::NONE;
	ClickStartInRadius = EClickStartInRadius::NOT_STARTED;
}

void ACharacterDeployManager::ApplyDPChange(int32 ChangeDeploymentPointBy)
{
	int32 PrevPoint = DeploymentPoint;
	DeploymentPoint = FMath::Clamp(DeploymentPoint + ChangeDeploymentPointBy, MinDeploymentPoint, MaxDeploymentPoint);
	if (DeploymentPoint != PrevPoint)
	{
		OnDeploymentPointChange.Broadcast(DeploymentPoint);
	}
}

FVector ACharacterDeployManager::GetOrthoProjPosOnDeploySelectPlane(FVector SrcPos)
{
	return FMath::RayPlaneIntersection(SrcPos, DeploySelectPlane.GetNormal(), DeploySelectPlane);
}

void ACharacterDeployManager::FocusOnCharacter(ADeployableCharacter* FocusChar)
{
	if (DeployPhase == ECharacterDeployPhase::NONE)
	{
		DeployPhase = ECharacterDeployPhase::FOCUSING;
		FocusingOnChar = FocusChar;
		bIsMouseHolding = true; // This stops Tick function from treating the click to focus as subsequent clicks that may unfocus the char
		UILogicFocusOnCharacter(FocusChar);
	}
}

void ACharacterDeployManager::Unfocus()
{
	UILogicUnfocus();
	ResetDeployPhase();
}
