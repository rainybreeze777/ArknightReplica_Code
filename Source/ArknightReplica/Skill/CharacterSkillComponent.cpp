// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterSkillComponent.h"
#include "../Skill/AbstractSkillEffectBase.h"

static const float FLOAT_TOLERANCE = 0.00001f;

// Sets default values for this component's properties
UCharacterSkillComponent::UCharacterSkillComponent()
	: SkillDuration(-1.0f)
	, bIsAutoCast(false)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UCharacterSkillComponent::BeginPlay()
{
	Super::BeginPlay();

	CurrentSkillPoint = InitialSkillPoint;
}


// Called every frame
void UCharacterSkillComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (RecoveryMode == ESkillRecoveryMode::AUTOMATIC)
	{
		float SpChange = DeltaTime;
		if (SkillChangeState == ESkillPointsChangeState::GROWING)
		{
			ChangeSkillPoint(SpChange);
		}
	}

	if (SkillChangeState == ESkillPointsChangeState::CONSUMING)
	{
		SkillDurationLeft = FMath::Clamp(SkillDurationLeft - DeltaTime, 0.0f, SkillDuration);
		if (SkillDurationLeft <= FLOAT_TOLERANCE)
		{
			OnSkillPointsDepleted.Broadcast();
			if (RecoveryMode == ESkillRecoveryMode::AUTOMATIC)
			{
				StartSkillPointsRecovery();
			}
		}
	}
}

void UCharacterSkillComponent::ExecuteSkill()
{
#if WITH_EDITOR
	checkf(IsValid(CharSkill.GetObject()), TEXT("Skill in Character Skill Component of %s is not valid! Did you forget to assign a skill?"), *(GetOwner()->GetActorLabel()));
#endif
	CharSkill->Execute_ExecuteSkill(Cast<UObject>(CharSkill.GetObject()));
}

void UCharacterSkillComponent::ApplySkillEffects()
{
#if WITH_EDITOR
	checkf(IsValid(CharSkill.GetObject()), TEXT("Skill in Character Skill Component of %s is not valid! Did you forget to assign a skill?"), *(GetOwner()->GetActorLabel()));
#endif
	CharSkill->Execute_ApplySkillEffect(Cast<UObject>(CharSkill.GetObject()));
}

void UCharacterSkillComponent::ChangeSkillPoint(float ChangeBy)
{
	float PrevSkillPoint = CurrentSkillPoint;
	CurrentSkillPoint = FMath::Clamp(CurrentSkillPoint + ChangeBy, 0.0f, GetSkillPointCost());
	if (!FMath::IsNearlyEqual(CurrentSkillPoint, PrevSkillPoint, FLOAT_TOLERANCE))
	{
		SkillPointChangeDelegate.Broadcast(CurrentSkillPoint);
	}
	if (FMath::IsNearlyEqual(CurrentSkillPoint, GetSkillPointCost(), FLOAT_TOLERANCE))
	{
		UpdateSkillPointsChangeState(ESkillPointsChangeState::UNCHANGING);
		CurrentSkillPoint = GetSkillPointCost();
	}
}

void UCharacterSkillComponent::AssignSkill(UAbstractSkillEffectBase* InSkillEffect)
{
	check(InSkillEffect->GetClass()->ImplementsInterface(USkillEffect::StaticClass()));
	CharSkill.SetInterface(Cast<ISkillEffect>(InSkillEffect));
	CharSkill.SetObject(InSkillEffect);
}

void UCharacterSkillComponent::ConsumeSkillPoints()
{
	if (HasEnoughPointsToCast())
	{
		if (SkillDuration >= FLOAT_TOLERANCE)
		{
			SkillDurationLeft = SkillDuration;
		}

		UpdateSkillPointsChangeState(ESkillPointsChangeState::CONSUMING);
		ChangeSkillPoint(-1 * GetSkillPointCost());
	}
}

void UCharacterSkillComponent::StartSkillPointsRecovery()
{
	if (RecoveryMode == ESkillRecoveryMode::AUTOMATIC)
	{
		UpdateSkillPointsChangeState(ESkillPointsChangeState::GROWING);
	}
}

void UCharacterSkillComponent::UpdateSkillPointsChangeState(ESkillPointsChangeState NewState)
{
	SkillChangeState = NewState;
	OnSkillPointsChangeStateUpdate.Broadcast(SkillChangeState);
}

float UCharacterSkillComponent::GetSkillPointCost() const
{
	return CharSkill->Execute_GetSkillCost(Cast<UObject>(CharSkill.GetObject()));
}

bool UCharacterSkillComponent::HasEnoughPointsToCast() const
{
	return CurrentSkillPoint >= GetSkillPointCost();
}
