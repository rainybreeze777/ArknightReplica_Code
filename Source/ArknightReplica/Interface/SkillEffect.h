// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SkillEffect.generated.h"

/**
 *
 */
UINTERFACE(MinimalAPI, Blueprintable)
class USkillEffect : public UInterface
{
	GENERATED_BODY()
};

class ISkillEffect
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Skill Effect")
	void ExecuteSkill();
	virtual void ExecuteSkill_Implementation();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Skill Effect")
	float GetSkillCost() const;
	virtual float GetSkillCost_Implementation() const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Skill Effect")
	void ApplySkillEffect();
	virtual void ApplySkillEffect_Implementation();
};
