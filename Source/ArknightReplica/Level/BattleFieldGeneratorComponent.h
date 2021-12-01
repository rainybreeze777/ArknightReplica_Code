// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BattleFieldGeneratorComponent.generated.h"

class ABattleField;
class AStaticMeshActor;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ARKNIGHTREPLICA_API UBattleFieldGeneratorComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UBattleFieldGeneratorComponent();

	UPROPERTY(Transient, EditInstanceOnly, Category="Battlefield Generation")
	uint8 SetRowTo;

	UPROPERTY(Transient, EditInstanceOnly, Category="Battlefield Generation")
	uint8 SetColumnTo;

	UPROPERTY(Transient, EditInstanceOnly, Category = "Battlefield Generation", meta=(DisplayName="Cell Width"))
	float CellScale;

	UPROPERTY(EditInstanceOnly, Category="Battlefield Generation")
	TSubclassOf<AStaticMeshActor> FieldCell;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	ABattleField* BattleField;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void SetBattleField(ABattleField* InBattlefield);

	UFUNCTION(CallInEditor, Category="Battlefield Generation")
	void GenerateBattleField();

	UFUNCTION(CallInEditor, Category = "Battlefield Generation")
	void ResetBattleField();
};
