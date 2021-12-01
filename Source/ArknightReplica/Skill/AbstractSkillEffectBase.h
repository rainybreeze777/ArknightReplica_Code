// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Interface/SkillEffect.h"
#include "AbstractSkillEffectBase.generated.h"

class ADeployableCharacter;

/**
 * Abstract base class to be extended by all skill effect and skill decorator
 * subclasses.
 */
UCLASS(Blueprintable, Abstract)
class ARKNIGHTREPLICA_API UAbstractSkillEffectBase : public UObject, public ISkillEffect
{
	GENERATED_BODY()

protected:
	UPROPERTY(Transient, BlueprintReadWrite)
	ADeployableCharacter* SkillOwner;

public:
	UFUNCTION(BlueprintCallable)
	void InitSkillEffect(ADeployableCharacter* InSkillOwner);
};
