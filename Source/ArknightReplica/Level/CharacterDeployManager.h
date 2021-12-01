// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CharacterDeployManager.generated.h"

class AOverseerControllerBase;
class ABattleField;
enum class ECharDeployOrientation : uint8;

UENUM(BlueprintType)
enum class ECharacterDeployPhase : uint8
{
	NONE					UMETA(DisplayName = "None"),
	POSITION			UMETA(DisplayName = "Position"),
	ORIENTATION		UMETA(DisplayName = "Orientation"),
	FOCUSING			UMETA(DisplayName = "Focusing"),
};

UENUM(BlueprintType)
enum class EClickStartInRadius : uint8
{
	NOT_STARTED,
	YES,
	NO
};

UCLASS()
class ARKNIGHTREPLICA_API ACharacterDeployManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACharacterDeployManager();

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDPChangeDel, int32, CurrentDeploymentPoint);

	UPROPERTY(BlueprintAssignable)
	FOnDPChangeDel OnDeploymentPointChange;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 StartingDeploymentPoint;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 MaxDeploymentPoint;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 MinDeploymentPoint;

protected:
	UPROPERTY(BlueprintReadOnly, EditInstanceOnly)
	ABattleField* BattleField;

	UPROPERTY(BlueprintReadOnly)
	AOverseerControllerBase* OverseerController;

	UPROPERTY(BlueprintReadWrite, VisibleInstanceOnly)
	ECharacterDeployPhase DeployPhase;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float DeploySelectPlaneHeight;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float DeploySelectRadius;

	UPROPERTY(BlueprintReadWrite)
	FTimerHandle DeploymentPointRecoverHandle;

	UPROPERTY(Transient, BlueprintReadOnly)
	ADeployableCharacter* StagingChar;

	UPROPERTY(Transient, BlueprintReadWrite)
	ADeployableCharacter* FocusingOnChar;

	UPROPERTY(Transient, BlueprintReadOnly)
	FVector CursorVector;

	UPROPERTY(Transient, BlueprintReadOnly)
	EClickStartInRadius ClickStartInRadius;

	UPROPERTY(Transient, BlueprintReadOnly)
	FPlane DeploySelectPlane;

	UPROPERTY(Transient, BlueprintReadWrite, VisibleInstanceOnly)
	int32 DeploymentPoint;

private:
	bool bIsMouseHolding;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void InitiateDeployment(TSubclassOf<ADeployableCharacter> CharToDeploy);

	UFUNCTION(BlueprintCallable)
	void FocusOnCharacter(ADeployableCharacter* FocusChar);

	UFUNCTION(BlueprintCallable)
	void Unfocus();

	UFUNCTION(BlueprintCallable)
	void ApplyDPChange(int32 ChangeDeploymentPointBy);

	UFUNCTION(BlueprintPure)
	ECharacterDeployPhase GetCurrentDeployPhase() const { return DeployPhase; }

	UFUNCTION(BlueprintPure)
	FORCEINLINE int32 GetCurrentDP() const { return DeploymentPoint; }

	UFUNCTION(BlueprintPure)
	FVector GetOrthoProjPosOnDeploySelectPlane(FVector SrcPos);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintImplementableEvent, meta = (BlueprintInternalUseOnly))
	void UILogicFocusOnCharacter(ADeployableCharacter* FocusChar);

	UFUNCTION(BlueprintImplementableEvent, meta = (BlueprintInternalUseOnly))
	void UILogicUnfocus();

	UFUNCTION(BlueprintImplementableEvent, meta = (BlueprintInternalUseOnly))
	void UILogicDeployEnterOrientationPhase();

	UFUNCTION(BlueprintImplementableEvent, meta = (BlueprintInternalUseOnly))
	void UILogicUpdateDeployingOrientation(ECharDeployOrientation Orientation);

	UFUNCTION(BlueprintImplementableEvent, meta = (BlueprintInternalUseOnly))
	void UILogicUpdateCursorDrag(bool bIsDragging, FVector InDraggingStart, FVector InCursorVector, float InMaxLength);

private:
	UFUNCTION()
	ABattleFieldCell* AdjustDeployPosition();

	UFUNCTION()
	ECharDeployOrientation AdjustDeployOrientation(FVector InCursorVector, EClickStartInRadius InClickStartInRadius, float InSelectRadius);

	UFUNCTION()
	void ResetDeployPhase();
};
