// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameInstance.h"

#include "JsonObjectConverter.h"

UMyGameInstance::UMyGameInstance()
{
	PlayerInfo.Name = "";
	PlayerInfo.Score = 0;
}

void UMyGameInstance::SetPlayerInfoScore(int Score)
{
	PlayerInfo.Score = Score;
}

void UMyGameInstance::SetPlayerInfoName(FString Name)
{
	PlayerInfo.Name = Name;
}

int UMyGameInstance::GetPlayerInfoScore()
{
	return PlayerInfo.Score;
}

FString UMyGameInstance::GetPlayerInfoName()
{
	return PlayerInfo.Name;
}

bool UMyGameInstance::SavePlayerInfo()
{
	FString JsonStr="";
	FString Path = FPaths::ProjectDir() + "SaveFile/PlayerInfo.Json";

	FFileHelper::LoadFileToString(JsonStr, *Path);
	TSharedPtr<FJsonObject> rootObject = MakeShareable(new FJsonObject());
	TSharedRef<TJsonReader<>> jsonReader = TJsonReaderFactory<>::Create(JsonStr);

	//声明PlayerInfo的数组
	TArray<TSharedPtr<FJsonValue>> PlayerNameList;
	TArray<TSharedPtr<FJsonValue>> PlayerScoreList;

	if (FJsonSerializer::Deserialize(jsonReader, rootObject))
	{
		PlayerNameList = rootObject->GetArrayField("PlayerNames");
		PlayerScoreList = rootObject->GetArrayField("PlayerScores");
	}
	
	
	//获取当前的PlayerInfo
	FString PlayerName = PlayerInfo.Name;
	int PlayerScore = PlayerInfo.Score;

	//将现在的PlayerInfo添加进表里
	TSharedPtr<FJsonValueString> Name = MakeShareable(new FJsonValueString(PlayerName));
	TSharedPtr<FJsonValueNumber> Score = MakeShareable(new FJsonValueNumber(PlayerScore));
	PlayerNameList.Add(Name);
	PlayerScoreList.Add(Score);
	
	TSharedRef<TJsonWriter<>> JsonWriter = TJsonWriterFactory<>::Create(&JsonStr);
	TSharedPtr<FJsonObject> RootObject = MakeShareable(new FJsonObject());

	RootObject->SetArrayField(TEXT("PlayerNames"),PlayerNameList);
	RootObject->SetArrayField(TEXT("PlayerScores"),PlayerScoreList);
	
	FJsonSerializer::Serialize(RootObject.ToSharedRef(), JsonWriter);
	return FFileHelper::SaveStringToFile(JsonStr, *Path);
}

bool UMyGameInstance::LoadPlayerInfo()
{
	//查询战绩UserWidget返回大厅会直接清理数组
	//PlayerInfoList.Empty();
	
	FString JsonStr = "";
	FString Path = FPaths::ProjectDir() + "SaveFile/PlayerInfo.Json";
	FFileHelper::LoadFileToString(JsonStr, *Path);
	TSharedPtr<FJsonObject> rootObject = MakeShareable(new FJsonObject());
	TSharedRef<TJsonReader<>> jsonReader = TJsonReaderFactory<>::Create(JsonStr);

	TArray<TSharedPtr<FJsonValue>> PlayerNameList;
	TArray<TSharedPtr<FJsonValue>> PlayerScoreList;

	if (FJsonSerializer::Deserialize(jsonReader, rootObject))
	{
		PlayerNameList = rootObject->GetArrayField("PlayerNames");
		PlayerScoreList = rootObject->GetArrayField("PlayerScores");
	}

	for (int i = 0; i < PlayerNameList.Num(); i++)
	{
		FPlayerInfo PlayerInfoTemp;
		PlayerInfoTemp.Name = PlayerNameList[i]->AsString();
		PlayerInfoTemp.Score = PlayerScoreList[i]->AsNumber();
		PlayerInfoList.Add(PlayerInfoTemp);
	}

	for (int i = 0; i < PlayerInfoList.Num(); i++)
	{
		FString Name = PlayerInfoList[i].Name;
		int Score = PlayerInfoList[i].Score;
		UE_LOG(LogTemp, Warning, TEXT("%s"),*Name);
		UE_LOG(LogTemp, Warning, TEXT("%d"),Score);
	}
	
	UE_LOG(LogTemp, Warning, TEXT("%s"),*JsonStr);
	return true;
}
