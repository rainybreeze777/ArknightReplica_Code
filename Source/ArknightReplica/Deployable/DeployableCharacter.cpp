// Fill out your copyright notice in the Description page of Project Settings.


#include "DeployableCharacter.h"
#include "../Level/BattleFieldCell.h"
#include "../Enemy/EnemyBase.h"
#include "../PlayerState/DefaultPlayerStateBase.h"
#include "../Level/CharacterDeployManager.h"
#include "../Skill/CharacterSkillComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ADeployableCharacter::ADeployableCharacter()
	: bIsDying(false)
	, bIsBasicAttackInCooldown(false)
	, bIsInDeployStage(true)
	, bIsCastingSkill(false)
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ADeployableCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	if (!IsValid(BattleField))
	{
		BattleField = Cast<ABattleField>(UGameplayStatics::GetActorOfClass(GetWorld(), ABattleField::StaticClass()));
	}
	CurrentStats = FCharStats(DefaultStats);

#if WITH_EDITOR
	ensureAlwaysMsgf(DeployableSurfaceTypes, TEXT("%s should not have NONE as deployable surface type!"), *GetActorLabel());
#endif

	// When spawned, character is in deploy stage, which means no animation,
	// collision, physics movement and action
	GetMesh()->bPauseAnims = true;
	SetActorEnableCollision(false);

	// Temporary Hack to find reference to PlayerState at game start
	ArknightPlayerState = Cast<ADefaultPlayerStateBase>(UGameplayStatics::GetActorOfClass(GetWorld(), ADefaultPlayerStateBase::StaticClass()));
	OnClicked.AddDynamic(this, &ADeployableCharacter::OnClickFocusCharacter);

	SkillComp = FindComponentByClass<UCharacterSkillComponent>();
	if (IsValid(SkillComp))
	{
		SkillComp->OnSkillPointsDepleted.AddDynamic(this, &ADeployableCharacter::ExitSkillCasting);
	}
}

// Called every frame
void ADeployableCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADeployableCharacter::CommitDeploy()
{
	GetMesh()->bPauseAnims = false;
	SetActorEnableCollision(true);
	NotifyDeployDelegate.Broadcast();
	// Let AI take over
	ComputedWithinRangeCells = GetWithinRangeCells();
	SpawnDefaultController();
	if (IsValid(SkillComp))
	{
		SkillComp->AssignSkill(ConstructSkillEffect());
		SkillComp->StartSkillPointsRecovery();
	}
	DoPostDeployCommitBlueprint();
}

FVector ADeployableCharacter::GetDeployPositionForPoint(FVector StandingPoint) const
{
	UCapsuleComponent* ColliderCapsule = GetCapsuleComponent();
	float HalfHeight = ColliderCapsule->GetScaledCapsuleHalfHeight();
	return StandingPoint + FVector(0, 0, HalfHeight);
}

int32 ADeployableCharacter::GetDeployCost() const
{
	return CurrentStats.DeployCost;
}

TArray<ABattleFieldCell*> ADeployableCharacter::GetWithinRangeCells() const
{
	if (DeployOrientation == ECharDeployOrientation::NONE)
	{
#if WITH_EDITOR
		UE_LOG(LogTemp, Error, TEXT("DeployOrientation of character %s is not set!"), *GetActorLabel());
#endif
		return TArray<ABattleFieldCell*>();
	}

	TArray<ABattleFieldCell*> WithinRangeCells;

	for (auto Coord : AttackRange)
	{
		FIntPoint CellCoord(CharLocation);
		switch (DeployOrientation)
		{
			case ECharDeployOrientation::NORTH:
				// North facing means x,y are flipped, then new y is negated
				CellCoord += FIntPoint(Coord.Y, Coord.X * -1);
				break;
			case ECharDeployOrientation::SOUTH:
				// South facing means x,y are flipped
				CellCoord += FIntPoint(Coord.Y, Coord.X);
				break;
			case ECharDeployOrientation::WEST:
				// West facing means x is negated
				CellCoord += FIntPoint(Coord.X * -1, Coord.Y);
				break;
			default:
				// EAST facing remains the same
				CellCoord += Coord;
				break;
		}
		if (IsCoordValid(CellCoord))
		{
			WithinRangeCells.Add((*BattleField)[CellCoord.Y][CellCoord.X]);
		}
	}

	return WithinRangeCells;
}

TArray<AEnemyBase*> ADeployableCharacter::GetWithinRangeEnemies() const
{
	TArray<AEnemyBase*> WithinRangeEnemies;
	const TArray<AEnemyBase*>& AllAliveEnemies = BattleField->GetAliveEnemies();
	for (AEnemyBase* Enemy : AllAliveEnemies)
	{
		for (ABattleFieldCell* Cell : ComputedWithinRangeCells)
		{
			if (Cell->IsWithinCell(Enemy->GetActorLocation()))
			{
				WithinRangeEnemies.Add(Enemy);
			}
		}
	}
	return WithinRangeEnemies;
}

void ADeployableCharacter::BlockEnemy(AEnemyBase* Enemy)
{
	if (BlockingEnemies.Num() < CurrentStats.MaxBlockCount && !BlockingEnemies.Contains(Enemy))
	{
		bool BlockSuccessful = Enemy->SetBlocked(this);
		if (BlockSuccessful)
		{
			BlockingEnemies.Add(Enemy);
		}
	}
}

void ADeployableCharacter::OnEnemyDeath(AEnemyBase* DyingEnemy)
{
	if (BlockingEnemies.Contains(DyingEnemy))
	{
		BlockingEnemies.Remove(DyingEnemy);
	}
}

bool ADeployableCharacter::HasEnemiesInRange() const
{
#if WITH_EDITOR
	if (!GWorld->HasBegunPlay())
	{
		return false;
	}
	else
	{
		return GetWithinRangeEnemies().Num() > 0;
	}
#else
	return GetWithinRangeEnemies().Num() > 0;
#endif // WITH_EDITOR
}

void ADeployableCharacter::SetDeployOrientation(ECharDeployOrientation Orientation)
{
	DeployOrientation = Orientation;
	switch (Orientation)
	{
		case ECharDeployOrientation::EAST:
			SetActorLocationAndRotation(GetActorLocation(), FRotator(0, 0, 0), false, nullptr, ETeleportType::TeleportPhysics);
			break;
		case ECharDeployOrientation::SOUTH:
			SetActorLocationAndRotation(GetActorLocation(), FRotator(0, 90, 0), false, nullptr, ETeleportType::TeleportPhysics);
			break;
		case ECharDeployOrientation::WEST:
			SetActorLocationAndRotation(GetActorLocation(), FRotator(0, 180, 0), false, nullptr, ETeleportType::TeleportPhysics);
			break;
		case ECharDeployOrientation::NORTH:
			SetActorLocationAndRotation(GetActorLocation(), FRotator(0, 270, 0), false, nullptr, ETeleportType::TeleportPhysics);
			break;
		default:
			break;
	}
}

UAbstractSkillEffectBase* ADeployableCharacter::ConstructSkillEffect_Implementation()
{
	return nullptr;
}

bool ADeployableCharacter::IsAnimInterruptible_Implementation() const
{
	return true;
}

void ADeployableCharacter::ApplySkillEffects()
{
	if (IsValid(SkillComp))
	{
		SkillComp->ApplySkillEffects();
	}
}

void ADeployableCharacter::ConsumeSkillPoints()
{
	if (IsValid(SkillComp))
	{
		SkillComp->ConsumeSkillPoints();
	}
}

void ADeployableCharacter::EnterSkillCasting(bool bEnhanceBasicAttack, float InDamageModifier /* = 1.0f*/)
{
#if WITH_EDITOR
	checkf(IsValid(SkillComp), TEXT("Skill Component in Character %s is not valid!"), *GetActorLabel());
#endif
	bIsCastingSkill = true;
	bIsEnhancingBasicAttack = bEnhanceBasicAttack;
	if (bEnhanceBasicAttack)
	{
		DamageModifier = InDamageModifier;
		CurrentStats.Damage *= InDamageModifier;
	}
	StartCastingSkillDelegate.Broadcast();
}

void ADeployableCharacter::ExitSkillCasting()
{
#if WITH_EDITOR
	checkf(IsValid(SkillComp), TEXT("Skill Component in Character %s is not valid!"), *GetActorLabel());
#endif
	bIsCastingSkill = false;
	if (bIsEnhancingBasicAttack)
	{
		CurrentStats.Damage /= DamageModifier;
	}
	bIsEnhancingBasicAttack = false;
	EndCastingSkillDelegate.Broadcast();
}

void ADeployableCharacter::ExecuteSkill()
{
	if (IsValid(SkillComp))
	{
		SkillComp->ExecuteSkill();
	}
}

bool ADeployableCharacter::CanCastSkill() const
{
	if (!IsValid(SkillComp))
	{
		return false;
	}
	return SkillComp->HasEnoughPointsToCast();
}

bool ADeployableCharacter::IsSkillCastAutomatic() const
{
	if (!IsValid(SkillComp))
	{
		return false;
	}
	return SkillComp->IsAutoCast();
}

bool ADeployableCharacter::CanAttack() const
{
	return Execute_CanAttackInternal(Cast<UObject>(this));
}

bool ADeployableCharacter::CanAttackInternal_Implementation() const
{
	bool bCanAttack = !IsDying() && !bIsBasicAttackInCooldown;

	if (IsCastingSkill())
	{
		bCanAttack &= bIsEnhancingBasicAttack;
	}

	return bCanAttack;
}

void ADeployableCharacter::InitiateAttack_Implementation(ACharacter* Target)
{
	IBattleFieldEntity* AttackTarget = Cast<IBattleFieldEntity>(Target);
	if (AttackTarget != nullptr && CanAttack())
	{
		AttackingTargets.Add(AttackTarget);
		DoAttack();
	}
}

void ADeployableCharacter::InitiateAttackMultiTarget_Implementation(const TArray<ACharacter*>& ListOfTargets)
{
	if (CanAttack())
	{
		for (ACharacter* Char : ListOfTargets)
		{
			IBattleFieldEntity* AttackTarget = Cast<IBattleFieldEntity>(Char);
			if (AttackTarget != nullptr)
			{
				AttackingTargets.Add(AttackTarget);
			}
		}

		if (AttackingTargets.Num() > 0)
		{
			DoAttack();
		}
	}
}

void ADeployableCharacter::FinishAttack_Implementation()
{
	ClearAttackingTargets();
}

bool ADeployableCharacter::OnReceiveDamage_Implementation(float RawDamage, FHitVFXData& HitVFXData)
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

bool ADeployableCharacter::IsDying() const
{
	return Execute_IsDyingInternal(Cast<UObject>(this));
}

bool ADeployableCharacter::IsDyingInternal_Implementation() const
{
	return bIsDying;
}

void ADeployableCharacter::StartDying_Implementation()
{
	bIsDying = true;
	GetController()->PawnPendingDestroy(this);
	for (AEnemyBase* Enemy : BlockingEnemies)
	{
		Enemy->ClearBlocked();
	}
	StartDyingDelegate.Broadcast();
	SetActorEnableCollision(false);
}

void ADeployableCharacter::FinishDying_Implementation()
{
	Destroy();
}

void ADeployableCharacter::ReceiveDamage(float RawDamage)
{
	CurrentStats.Health -= RawDamage;
	HealthPointChangeDelegate.Broadcast(CurrentStats.Health);
}

void ADeployableCharacter::DoAttack()
{
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

void ADeployableCharacter::DoDamageTo(UObject* EnemyEntity, float Damage)
{
	if (EnemyEntity->GetClass()->ImplementsInterface(UBattleFieldEntity::StaticClass()))
	{
		IBattleFieldEntity* Enemy = Cast<IBattleFieldEntity>(EnemyEntity);
		FHitVFXData HitVFXData;
		HitVFXData.HitOrigin = GetActorLocation();
		bool IsKillingBlow = Enemy->Execute_OnReceiveDamage(EnemyEntity, Damage, HitVFXData);
		if (IsKillingBlow)
		{
			ArknightPlayerState->IncrementKillCount();
		}
		HitVFXData.HitDirection = HitVFXData.HitLocation - HitVFXData.HitOrigin;
		Execute_PlayDealDamageVFX(Cast<UObject>(this), HitVFXData);
	}
}

void ADeployableCharacter::DealDamage()
{
// #if UE_BUILD_DEVELOPMENT
// 	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, "Deal Damage");
// #endif
	for (IBattleFieldEntity* Enemy : AttackingTargets)
	{
		FHitVFXData HitVFXData;
		HitVFXData.HitOrigin = GetActorLocation();
		bool IsKillingBlow = Enemy->Execute_OnReceiveDamage(Cast<UObject>(Enemy), CurrentStats.Damage, HitVFXData);
		if (IsKillingBlow)
		{
			ArknightPlayerState->IncrementKillCount();
		}
		HitVFXData.HitDirection = HitVFXData.HitLocation - HitVFXData.HitOrigin;
		Execute_PlayDealDamageVFX(Cast<UObject>(this), HitVFXData);
	}
}

void ADeployableCharacter::ClearAttackingTargets()
{
	AttackingTargets.Empty();
}

void ADeployableCharacter::OnClickFocusCharacter(AActor* TouchedActor, FKey ButtonPressed)
{
	if (ButtonPressed == EKeys::LeftMouseButton)
	{
		DeployManager->FocusOnCharacter(this);
	}
}
