// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "../Interface/BattleFieldEntity.h"
#include "EnemyBase.generated.h"

class ADeployableCharacter;
class ABattleField;

USTRUCT(BlueprintType)
struct FEnemyStats
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float Damage;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float AttackInterval;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float Health;
};

UCLASS()
class ARKNIGHTREPLICA_API AEnemyBase : public ACharacter, public IBattleFieldEntity
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyBase();

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FEnemyStats DefaultStats;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FStartAttackDel);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FStartDyingDel);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FHealthPointChangeDel, float, NewHealthPoint);

	UPROPERTY(BlueprintAssignable)
	FHealthPointChangeDel HealthPointChangeDelegate;

protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bCanBeBlocked;

	UPROPERTY(Transient, BlueprintReadWrite, EditInstanceOnly)
	FEnemyStats CurrentStats;

	UPROPERTY(BlueprintReadOnly)
	ABattleField* BattleField;

	UPROPERTY(BlueprintReadWrite)
	FTimerHandle BasicAttackCooldownTimerHandle;

	UPROPERTY(BlueprintAssignable)
	FStartAttackDel StartAttackDelegate;

	UPROPERTY(BlueprintAssignable)
	FStartDyingDel StartDyingDelegate;

private:
	bool bIsDying;
	bool bIsBeingBlocked;
	bool bIsBasicAttackInCooldown;
	ADeployableCharacter* BlockingChar;
	IBattleFieldEntity* AttackingTarget;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void ReceiveDamage(float RawDamage);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void SetBattleField(ABattleField* InBattleField);

	UFUNCTION(BlueprintPure)
	bool IsBeingBlocked() const;

	UFUNCTION(BlueprintPure)
	bool CanBeBlocked() const;

	UFUNCTION(BlueprintPure)
	ADeployableCharacter* GetBlockingChar() const;

	UFUNCTION(BlueprintCallable)
	bool SetBlocked(ADeployableCharacter* Blocker);

	UFUNCTION(BlueprintCallable)
	void ClearBlocked();

	UFUNCTION(BlueprintCallable)
	void Escape();

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
};
