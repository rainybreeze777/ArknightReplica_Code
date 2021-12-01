// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyBase.h"
#include "..\Deployable\DeployableCharacter.h"

// Sets default values
AEnemyBase::AEnemyBase()
		: bCanBeBlocked(true)
		, bIsDying(false)
		, bIsBeingBlocked(false)
		, bIsBasicAttackInCooldown(false)
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AEnemyBase::BeginPlay()
{
	Super::BeginPlay();
	CurrentStats = FEnemyStats(DefaultStats);
}

// Called every frame
void AEnemyBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AEnemyBase::SetBattleField(ABattleField* InBattleField)
{
	BattleField = InBattleField;
}

void AEnemyBase::ReceiveDamage(float RawDamage)
{
	CurrentStats.Health -= RawDamage;
	HealthPointChangeDelegate.Broadcast(CurrentStats.Health);
}

bool AEnemyBase::IsBeingBlocked() const
{
	return bIsBeingBlocked;
}

bool AEnemyBase::CanBeBlocked() const
{
	return bCanBeBlocked;
}

ADeployableCharacter* AEnemyBase::GetBlockingChar() const
{
	if (!CanBeBlocked() || !IsBeingBlocked())
	{
		return nullptr;
	}
	return BlockingChar;
}

bool AEnemyBase::SetBlocked(ADeployableCharacter* Blocker)
{
	if (!CanBeBlocked() || bIsBeingBlocked)
	{
		return false;
	}
	if (Blocker == nullptr)
	{
#if WITH_EDITOR
		UE_LOG(LogTemp, Error, TEXT("Setting %s blocking to true, but did not provide the blocker!"), *GetActorLabel());
#endif
		bIsBeingBlocked = false;
		return false;
	}
	bIsBeingBlocked = true;
	BlockingChar = Blocker;
	return true;
}

void AEnemyBase::ClearBlocked()
{
	bIsBeingBlocked = false;
	BlockingChar = nullptr;
}

void AEnemyBase::Escape()
{
	BattleField->NotifyDespawnEnemy(this);
	Destroy();
}

bool AEnemyBase::CanAttack() const
{
	return Execute_CanAttackInternal(Cast<UObject>(this));
}

bool AEnemyBase::CanAttackInternal_Implementation() const
{
	return !IsDying() && !bIsBasicAttackInCooldown;
}

void AEnemyBase::InitiateAttack_Implementation(ACharacter *Target)
{
	IBattleFieldEntity* AttackTarget = Cast<IBattleFieldEntity>(Target);
	if (AttackTarget != nullptr && CanAttack())
	{
		AttackingTarget = AttackTarget;
		bIsBasicAttackInCooldown = true;
		StartAttackDelegate.Broadcast();
		FTimerDelegate ReadyBasicAttackDel;
		ReadyBasicAttackDel.BindLambda([&bCD = bIsBasicAttackInCooldown]() { 
			bCD = false; 
		});
		GetWorldTimerManager().SetTimer(
			BasicAttackCooldownTimerHandle, 
			ReadyBasicAttackDel, 
			CurrentStats.AttackInterval,
			false);
	}
}

void AEnemyBase::InitiateAttackMultiTarget_Implementation(const TArray<ACharacter *> &ListOfTargets)
{
	// TODO
}

void AEnemyBase::FinishAttack_Implementation()
{
	FHitVFXData HitVFXData;
	HitVFXData.HitOrigin = GetActorLocation();
	AttackingTarget->Execute_OnReceiveDamage(Cast<UObject>(AttackingTarget), CurrentStats.Damage, HitVFXData);
	HitVFXData.HitDirection = HitVFXData.HitLocation - HitVFXData.HitOrigin;
	Execute_PlayDealDamageVFX(Cast<UObject>(this), HitVFXData);
}

bool AEnemyBase::OnReceiveDamage_Implementation(float RawDamage, FHitVFXData& HitVFXData)
{
	if (IsDying()) { return false; }
	HitVFXData.HitLocation = GetActorLocation();
	ReceiveDamage(RawDamage);
	if (CurrentStats.Health <= 0)
	{
		Execute_StartDying(Cast<UObject>(this));
		return true;
	}
	return false;
}

bool AEnemyBase::IsDying() const
{
	return Execute_IsDyingInternal(Cast<UObject>(this));
}

bool AEnemyBase::IsDyingInternal_Implementation() const
{
	return bIsDying;
}

void AEnemyBase::StartDying_Implementation()
{
	bIsDying = true;
	GetController()->PawnPendingDestroy(this);
	if (BlockingChar != nullptr)
	{
		BlockingChar->OnEnemyDeath(this);
	}
	BattleField->NotifyDespawnEnemy(this);
	StartDyingDelegate.Broadcast();
	SetActorEnableCollision(false);
}

void AEnemyBase::FinishDying_Implementation()
{
	Destroy();
}
