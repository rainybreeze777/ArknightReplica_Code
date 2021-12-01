// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../SharedStruct/HitVFXData.h"
#include "BattleFieldEntity.generated.h"

class ACharacter;

/**
 * 
 */
UINTERFACE(MinimalAPI, Blueprintable)
class UBattleFieldEntity : public UInterface
{
	GENERATED_BODY()
};

class IBattleFieldEntity
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Battle Field Entity", meta = (DisplayName = "CanAttack"))
	bool CanAttackInternal() const;
	virtual bool CanAttackInternal_Implementation() const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Battle Field Entity")
	void InitiateAttack(ACharacter* Target);
	virtual void InitiateAttack_Implementation(ACharacter* Target);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Battle Field Entity")
	void InitiateAttackMultiTarget(const TArray<ACharacter*>& ListOfTargets);
	virtual void InitiateAttackMultiTarget_Implementation(const TArray<ACharacter*>& ListOfTargets);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Battle Field Entity")
	void FinishAttack();
	virtual void FinishAttack_Implementation();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Battle Field Entity")
	bool OnReceiveDamage(float RawDamage, FHitVFXData& HitVFXData);
	virtual bool OnReceiveDamage_Implementation(float RawDamage, FHitVFXData& HitVFXData);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Battle Field Entity", meta = (DisplayName = "IsDying"))
	bool IsDyingInternal() const;
	virtual bool IsDyingInternal_Implementation() const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Battle Field Entity")
	void StartDying();
	virtual void StartDying_Implementation();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Battle Field Entity")
	void FinishDying();
	virtual void FinishDying_Implementation();

	UFUNCTION(BlueprintImplementableEvent)
	void PlayDealDamageVFX(const FHitVFXData& HitVFXData);
};
