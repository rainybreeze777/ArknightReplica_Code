#include "BattleFieldEntity.h"

bool IBattleFieldEntity::CanAttackInternal_Implementation() const { return false; }

void IBattleFieldEntity::InitiateAttack_Implementation(ACharacter* Target) {}

void IBattleFieldEntity::InitiateAttackMultiTarget_Implementation(const TArray<ACharacter*>& ListOfTargets) {}

void IBattleFieldEntity::FinishAttack_Implementation() {}

bool IBattleFieldEntity::OnReceiveDamage_Implementation(float RawDamage, FHitVFXData& HitVFXData) { return false; }

bool IBattleFieldEntity::IsDyingInternal_Implementation() const { return false; }

void IBattleFieldEntity::StartDying_Implementation() {}

void IBattleFieldEntity::FinishDying_Implementation() {}
