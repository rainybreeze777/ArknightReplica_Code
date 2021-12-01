// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbstractSkillEffectBase.h"
#include "SkillEffectDecorator.generated.h"

class ADeployableCharacter;
class ABattleField;

/**
 * 
 */
UCLASS(Blueprintable, Abstract)
class ARKNIGHTREPLICA_API USkillEffectDecorator : public UAbstractSkillEffectBase
{
	GENERATED_BODY()

protected:
	UPROPERTY(Transient, BlueprintReadWrite)
	TScriptInterface<ISkillEffect> BaseEffect;

	UPROPERTY(Transient, BlueprintReadWrite)
	ABattleField* BattleField;

public:
	UFUNCTION(BlueprintCallable)
	void InitDecorator(UAbstractSkillEffectBase* InBaseEffect, 
		ADeployableCharacter* InSkillOwner, 
		ABattleField* InBattleField);

	void ExecuteSkill_Implementation() override;
	float GetSkillCost_Implementation() const override;
	void ApplySkillEffect_Implementation() override;
};
