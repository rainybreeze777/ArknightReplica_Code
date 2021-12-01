#include "SkillEffectDecorator.h"

void USkillEffectDecorator::InitDecorator(
	UAbstractSkillEffectBase* InBaseEffect, 
	ADeployableCharacter* InSkillOwner, 
	ABattleField* InBattleField)
{
	check(InBaseEffect->GetClass()->ImplementsInterface(USkillEffect::StaticClass()));
	BaseEffect.SetInterface(Cast<ISkillEffect>(InBaseEffect));
	BaseEffect.SetObject(InBaseEffect);
	SkillOwner = InSkillOwner;
	BattleField = InBattleField;
}

void USkillEffectDecorator::ExecuteSkill_Implementation()
{
	checkf(BaseEffect != nullptr, TEXT("Skill Effect Decorator should not have null base effect! Did you forget to run InitDecorator?"));
	BaseEffect->Execute_ExecuteSkill(Cast<UObject>(BaseEffect.GetObject()));
}

float USkillEffectDecorator::GetSkillCost_Implementation() const
{
	checkf(BaseEffect != nullptr, TEXT("Skill Effect Decorator should not have null base effect! Did you forget to run InitDecorator?"));
	return BaseEffect->Execute_GetSkillCost(Cast<UObject>(BaseEffect.GetObject()));
}

void USkillEffectDecorator::ApplySkillEffect_Implementation()
{
	checkf(BaseEffect != nullptr, TEXT("Skill Effect Decorator should not have null base effect! Did you forget to run InitDecorator?"));
	BaseEffect->Execute_ApplySkillEffect(Cast<UObject>(BaseEffect.GetObject()));
}
