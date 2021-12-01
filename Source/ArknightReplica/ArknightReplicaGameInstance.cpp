// Fill out your copyright notice in the Description page of Project Settings.


#include "ArknightReplicaGameInstance.h"
#include "JsonObjectConverter.h"

void UArknightReplicaGameInstance::Init()
{
	Super::Init();

	const FString JsonFilePath = FPaths::ProjectContentDir() + LevelDataJsonPath;
	FString LevelDataJsonString;

	FFileHelper::LoadFileToString(LevelDataJsonString, *JsonFilePath);

	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());
	TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(LevelDataJsonString);

	if (FJsonSerializer::Deserialize(JsonReader, JsonObject) && JsonObject.IsValid())
	{
		for (auto& Pair : JsonObject->Values)
		{
			FString LevelName = Pair.Key;
			const TSharedPtr<FJsonObject>* LevelDataJsonObjectPtr;
			if (Pair.Value->TryGetObject(LevelDataJsonObjectPtr))
			{
				FLevelData LevelData;
				FJsonObjectConverter::JsonObjectToUStruct((*LevelDataJsonObjectPtr).ToSharedRef(), FLevelData::StaticStruct(), &LevelData);
				LevelDataMap.Add(LevelName, MoveTemp(LevelData));
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to deserialize %s"), *JsonFilePath);
	}
}

int32 UArknightReplicaGameInstance::GetLossAtEscapedEnemyCount(const FString& LevelName) const
{
	const FLevelData* FoundData = LevelDataMap.Find(LevelName);
	if (FoundData != nullptr)
	{
		return FoundData->LossEnemyCount;
	}
	return -1;
}
