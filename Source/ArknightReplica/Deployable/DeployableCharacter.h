// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Level/BattleField.h"
#include "../Interface/BattleFieldEntity.h"
#include "../Enum/DeploySurfaceType.h"
#include "GameFramework/Character.h"
#include "DeployableCharacter.generated.h"

class ABattleFieldCell;
class AEnemyBase;
class ADefaultPlayerStateBase;
class ACharacterDeployManager;
class UTexture;
class UCharacterSkillComponent;
class UAbstractSkillEffectBase;

UENUM(BlueprintType)
enum class ECharDeployOrientation : uint8
{
	NONE		UMETA(DisplayName = "None"),
	NORTH		UMETA(DisplayName = "North"),
	SOUTH		UMETA(DisplayName = "South"),
	WEST		UMETA(DisplayName = "West"),
	EAST		UMETA(DisplayName = "East"),
};

USTRUCT(BlueprintType)
struct FCharStats
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float Damage;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float AttackInterval;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float Health;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 MaxTargetCount;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 MaxBlockCount;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 DeployCost;
};

UCLASS()
class ARKNIGHTREPLICA_API ADeployableCharacter : public ACharacter, public IBattleFieldEntity
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ADeployableCharacter();

public:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	FCharStats DefaultStats;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	UTexture2D* ProfilePic;

	UPROPERTY(BlueprintReadOnly, EditInstanceOnly)
	ABattleField* BattleField;

	UPROPERTY(BlueprintReadWrite, VisibleInstanceOnly)
	FIntPoint CharLocation;

	UPROPERTY(BlueprintReadWrite, VisibleInstanceOnly)
	ECharDeployOrientation DeployOrientation;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (Bitmask, BitmaskEnum = EDeploySurfaceType))
	int32 DeployableSurfaceTypes;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	TArray<FIntPoint> AttackRange;

	UPROPERTY(Transient, BlueprintReadOnly)
	ACharacterDeployManager* DeployManager;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FStartAttackDel);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FStartDyingDel);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FStartCastingSkill);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FEndCastingSkill);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FNotifyDeployDel);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FHealthPointChangeDel, float, NewHealthPoint);

	UPROPERTY(BlueprintAssignable)
	FHealthPointChangeDel HealthPointChangeDelegate;

protected:
	UPROPERTY(Transient, BlueprintReadOnly)
	TArray<ABattleFieldCell*> ComputedWithinRangeCells;

	UPROPERTY(Transient, BlueprintReadOnly)
	TArray<AEnemyBase*> BlockingEnemies;

	UPROPERTY(Transient, BlueprintReadWrite, EditInstanceOnly)
	FCharStats CurrentStats;

	UPROPERTY(BlueprintReadWrite)
	FTimerHandle BasicAttackCooldownTimerHandle;

	UPROPERTY(BlueprintAssignable)
	FStartAttackDel StartAttackDelegate;

	UPROPERTY(BlueprintAssignable)
	FStartDyingDel StartDyingDelegate;

	UPROPERTY(BlueprintAssignable)
	FStartCastingSkill StartCastingSkillDelegate;

	UPROPERTY(BlueprintAssignable)
	FEndCastingSkill EndCastingSkillDelegate;

	UPROPERTY(BlueprintAssignable)
	FNotifyDeployDel NotifyDeployDelegate;

	UPROPERTY(Transient, BlueprintReadOnly)
	ADefaultPlayerStateBase* ArknightPlayerState;

	UPROPERTY(Transient, BlueprintReadOnly)
	UCharacterSkillComponent* SkillComp;

	TArray<IBattleFieldEntity*> AttackingTargets;

private:
	bool bIsDying;
	bool bIsBasicAttackInCooldown;
	bool bIsInDeployStage;
	bool bIsCastingSkill;
	bool bIsEnhancingBasicAttack;
	/** Bruteforce hack to implement a quick damage modifier logic */
	float DamageModifier;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void CommitDeploy();

	UFUNCTION(BlueprintPure)
	FVector GetDeployPositionForPoint(FVector StandingPoint) const;

	UFUNCTION(BlueprintPure)
	int32 GetDeployCost() const;

	UFUNCTION(BlueprintCallable)
	TArray<ABattleFieldCell*> GetWithinRangeCells() const;

	UFUNCTION(BlueprintCallable)
	TArray<AEnemyBase*> GetWithinRangeEnemies() const;

	UFUNCTION(BlueprintCallable)
	void BlockEnemy(AEnemyBase* Enemy);

	UFUNCTION(BlueprintCallable)
	void OnEnemyDeath(AEnemyBase* DyingEnemy);

	UFUNCTION(BlueprintPure)
	bool HasEnemiesInRange() const;

	UFUNCTION(BlueprintCallable)
	void SetDeployOrientation(ECharDeployOrientation Orientation);

	UFUNCTION(BlueprintCallable)
	void DealDamage();

	UFUNCTION(BlueprintCallable)
	void DoDamageTo(UObject* EnemyEntity, float Damage);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	UAbstractSkillEffectBase* ConstructSkillEffect();
	virtual UAbstractSkillEffectBase* ConstructSkillEffect_Implementation();

	UFUNCTION(BlueprintPure, BlueprintNativeEvent)
	bool IsAnimInterruptible() const;
	virtual bool IsAnimInterruptible_Implementation() const;

	UFUNCTION(BlueprintCallable)
	void ApplySkillEffects();

	UFUNCTION(BlueprintCallable)
	void ConsumeSkillPoints();

	UFUNCTION(BlueprintCallable)
	void EnterSkillCasting(bool bEnhanceBasicAttack, float InDamageModifier = 1.0f);

	UFUNCTION(BlueprintCallable)
	void ExitSkillCasting();

	UFUNCTION(BlueprintCallable)
	void ExecuteSkill();

	UFUNCTION(BlueprintPure)
	bool CanCastSkill() const;

	UFUNCTION(BlueprintPure)
	bool IsSkillCastAutomatic() const;

	UFUNCTION(BlueprintPure)
	FORCEINLINE int32 GetDeployableSurfaceTypes() const { return DeployableSurfaceTypes; }

	UFUNCTION(BlueprintPure)
	FORCEINLINE bool IsCastingSkill() const { return bIsCastingSkill; }

	UFUNCTION(BlueprintPure)
	FORCEINLINE bool IsEnhancingBasicAttack() const { return bIsEnhancingBasicAttack; }

	bool CanAttack() const;
	
	bool IsDying() const;

	//-===========================================================================
	// Interface implementations
	
	// BattleFieldEntity.h
	bool CanAttackInternal_Implementation() const override;
	void InitiateAttack_Implementation(ACharacter* Target) override;
	void InitiateAttackMultiTarget_Implementation(const TArray<ACharacter*>& ListOfTargets) override;
	void FinishAttack_Implementation() override;
	bool OnReceiveDamage_Implementation(float RawDamage, FHitVFXData& HitVFXData) override;
	bool IsDyingInternal_Implementation() const override;
	void StartDying_Implementation() override;
	void FinishDying_Implementation() override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void ReceiveDamage(float RawDamage);

	UFUNCTION(BlueprintCallable)
	void ClearAttackingTargets();

	UFUNCTION(BlueprintImplementableEvent)
	void DoPostDeployCommitBlueprint();

private:
	FORCEINLINE bool IsCoordValid(const FIntPoint& Coord) const 
	{ 
		return Coord.X >= 0 && Coord.X < BattleField->GetColumnCount() && Coord.Y >= 0 && Coord.Y < BattleField->GetRowCount();
	}

	FORCEINLINE bool IsCoordValid(int32 CoordX, int32 CoordY) const
	{
		return CoordX >= 0 && CoordX < BattleField->GetColumnCount() && CoordY >= 0 && CoordY < BattleField->GetRowCount();
	}

	void DoAttack();

	UFUNCTION()
	void OnClickFocusCharacter(AActor* TouchedActor, FKey ButtonPressed);
};
