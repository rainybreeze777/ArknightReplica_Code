#include "AbstractSkillEffectBase.h"
#include "../Deployable/DeployableCharacter.h"

void UAbstractSkillEffectBase::InitSkillEffect(ADeployableCharacter* InSkillOwner)
{
  SkillOwner = InSkillOwner;
}
