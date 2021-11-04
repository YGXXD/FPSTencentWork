// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MyGameInstance.generated.h"

/**
 * 
 */
USTRUCT()
struct FPlayerInfo
{
	GENERATED_BODY()

	FString Name;
	int Score;
	
};

UCLASS()
class TENCENT3_API UMyGameInstance : public UGameInstance
{
	GENERATED_BODY()
private:
	UMyGameInstance();
public:
	UPROPERTY()
	FPlayerInfo PlayerInfo;

	UPROPERTY()
	TArray<FPlayerInfo> PlayerInfoList;
	
	UFUNCTION(BlueprintCallable)
	void SetPlayerInfoName(FString Name);

	UFUNCTION(BlueprintCallable)
	void SetPlayerInfoScore(int Score);

	UFUNCTION(BlueprintCallable)
	FString GetPlayerInfoName();

	UFUNCTION(BlueprintCallable)
	int GetPlayerInfoScore();

	UFUNCTION(BlueprintCallable)
	bool SavePlayerInfo();

	UFUNCTION(BlueprintCallable)
	bool LoadPlayerInfo();
	
};
