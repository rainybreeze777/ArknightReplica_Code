// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyEscapeBoxComponent.h"
#include "../Enemy/EnemyBase.h"
#include "../GameState/LevelGameStateBase.h"

void UEnemyEscapeBoxComponent::BeginPlay()
{
	Super::BeginPlay();
	GameState = GetWorld()->GetGameState<ALevelGameStateBase>();

	OnComponentBeginOverlap.AddDynamic(this, &UEnemyEscapeBoxComponent::OnComponentBeginOverlapDel);
}

void UEnemyEscapeBoxComponent::OnComponentBeginOverlapDel(
			UPrimitiveComponent* OverlappedComponent, 
			AActor* OtherActor, 
			UPrimitiveComponent* OtherComp, 
			int32 OtherBodyIndex, 
			bool bFromSweep, 
			const FHitResult& SweepResult)
{
	AEnemyBase* Enemy = Cast<AEnemyBase>(OtherActor);
	if (Enemy != nullptr)
	{
		Enemy->Escape();
		GameState->OnEnemyEscape();
	}
}