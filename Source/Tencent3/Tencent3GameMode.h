// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UFOSpawnManager.h"
#include "UIGame.h"
#include "GameFramework/GameModeBase.h"
#include "Tencent3GameMode.generated.h"

UCLASS(minimalapi)
class ATencent3GameMode : public AGameModeBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Init")
	TSubclassOf<UUserWidget> GameUIClass;;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Init")
	TSubclassOf<UUserWidget> GameOverUIClass;;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Init")
	TSubclassOf<AUFOSpawnManager> UfoSpawnClass;;
	
	UPROPERTY()
	UUserWidget* CurrentWidget;

	UPROPERTY()
	UUIGame* GameWidget;

	UPROPERTY()
	AUFOSpawnManager* UFOSpawn;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Init")
	FVector UFOSpawnLocation;
private:
	float Time = 90;
	
	int Score = 0;

	bool bIsPlaying = true;

	UFUNCTION()
	void GameOver();
protected:
	virtual void Tick(float DeltaSeconds) override;
public:
	ATencent3GameMode();

	UFUNCTION()
	virtual void BeginPlay() override;

	UFUNCTION()
	void ChangeWidget(TSubclassOf<UUserWidget> NewWidgetClass);

	UFUNCTION()
	void AddScore(int value);
	
};



