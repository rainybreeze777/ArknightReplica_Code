// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "../Interface/SkillEffect.h"
#include "CharacterSkillComponent.generated.h"

class UAbstractSkillEffectBase;

UENUM(BlueprintType)
enum class ESkillRecoveryMode : uint8
{
	UNDEFINED		UMETA(DisplayName = "Undefined"),
	AUTOMATIC		UMETA(DisplayName = "Automatic"),
	OFFENSIVE		UMETA(DisplayName = "Offensive"),
	DEFENSIVE		UMETA(DisplayName = "Defensive")
};

UENUM(BlueprintType)
enum class ESkillPointsChangeState : uint8
{
	UNCHANGING		UMETA(DisplayName = "Unchanging"),
	GROWING				UMETA(DisplayName = "Growing"),
	CONSUMING			UMETA(DisplayName = "Consuming")
};

/**
 * 
 */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ARKNIGHTREPLICA_API UCharacterSkillComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCharacterSkillComponent();

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	UTexture2D* SkillIcon;

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float InitialSkillPoint;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	float SkillDuration;

	UPROPERTY(Transient, BlueprintReadWrite, VisibleInstanceOnly)
	float SkillDurationLeft;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FSkillPointsDepletedDel);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSkillPointChangeDel, float, NewSkillPoint);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSkillPointsChangeStateDel, ESkillPointsChangeState, NewSkillChangeState);

	UPROPERTY(BlueprintAssignable)
	FSkillPointChangeDel SkillPointChangeDelegate;

	UPROPERTY(BlueprintAssignable)
	FSkillPointsChangeStateDel OnSkillPointsChangeStateUpdate;

	UPROPERTY(BlueprintAssignable)
	FSkillPointsDepletedDel OnSkillPointsDepleted;

protected:
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	ESkillRecoveryMode RecoveryMode;

	UPROPERTY(Transient, BlueprintReadWrite, VisibleInstanceOnly)
	ESkillPointsChangeState SkillChangeState;

	UPROPERTY(Transient, BlueprintReadWrite, EditAnywhere)
	float CurrentSkillPoint;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	bool bIsAutoCast;

	UPROPERTY(Transient, BlueprintReadWrite)
	TScriptInterface<ISkillEffect> CharSkill;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void ExecuteSkill();

	UFUNCTION(BlueprintCallable)
	void ApplySkillEffects();

	UFUNCTION(BlueprintCallable)
	void ChangeSkillPoint(float ChangeBy);

	UFUNCTION(BlueprintCallable)
	void AssignSkill(UAbstractSkillEffectBase* InSkillEffect);

	UFUNCTION(BlueprintCallable)
	void ConsumeSkillPoints();

	UFUNCTION(BlueprintCallable)
	void StartSkillPointsRecovery();

	UFUNCTION(BlueprintCallable)
	void UpdateSkillPointsChangeState(ESkillPointsChangeState NewState);

	UFUNCTION(BlueprintPure)
	float GetSkillPointCost() const;

	UFUNCTION(BlueprintPure)
	bool HasEnoughPointsToCast() const;

	UFUNCTION(BlueprintPure)
	FORCEINLINE bool IsAutoCast() const { return bIsAutoCast; }
};
