// Copyright Epic Games, Inc. All Rights Reserved.

#include "Tencent3GameMode.h"

#include "MyGameInstance.h"
#include "Tencent3Character.h"
#include "UIGameOver.h"
#include "Blueprint/UserWidget.h"
#include "UObject/ConstructorHelpers.h"

ATencent3GameMode::ATencent3GameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/MyFile/BP/BP_MyTencent3Character"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
	PrimaryActorTick.bCanEverTick = true;
}

void ATencent3GameMode::BeginPlay()
{
	Super::BeginPlay();
	ChangeWidget(GameUIClass);
	GameWidget = Cast<UUIGame>(CurrentWidget);
	if(GameWidget)
	{
		GameWidget->UpdateScore(this->Score);
		GameWidget->UpdateTime(this->Time);
	}
	UWorld* World = GetWorld();
	if (World)
	{
		UFOSpawn = World->SpawnActor<AUFOSpawnManager>(UfoSpawnClass,UFOSpawnLocation,FRotator());
	}
}

void ATencent3GameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if(bIsPlaying)
	{
		if(Time>0.0f)
		{
			this->Time -= DeltaSeconds;
			if(GameWidget)
			{
				GameWidget->UpdateTime(this->Time);
			}
		}
		else
		{
			GameOver();
		}
	}
}

void ATencent3GameMode::GameOver()
{
	Time = 0;
	if(GameWidget)
	{
		GameWidget->UpdateTime(this->Time);
	}
	bIsPlaying = false;
	ChangeWidget(GameOverUIClass);

	UMyGameInstance* GameInstance = Cast<UMyGameInstance>(GetGameInstance());
	if(GameInstance)
	{
		GameInstance->SetPlayerInfoScore(this->Score);
		GameInstance->SavePlayerInfo();
	}
	
	UUIGameOver* GameOverWidget = Cast<UUIGameOver>(CurrentWidget);
	if(GameOverWidget)
	{
		GetWorld()->DestroyActor(UFOSpawn);
	}
	
	if(GameOverWidget)
	{
		GameOverWidget->SetScoreText(GameInstance->PlayerInfo.Score);
	}
}


void ATencent3GameMode::ChangeWidget(TSubclassOf<UUserWidget> NewWidgetClass)//切换UI界面
{
	if(CurrentWidget)
	{
		CurrentWidget->RemoveFromParent();
		CurrentWidget = nullptr;
	}
	if(NewWidgetClass)
	{
		CurrentWidget = CreateWidget<UUserWidget>(GetWorld(),NewWidgetClass);
		if(CurrentWidget != nullptr)
		{
			CurrentWidget->AddToViewport();
		}
	}
}

void ATencent3GameMode::AddScore(int value)
{
	Score += value;
	if(GameWidget)
	{
		GameWidget->UpdateScore(Score);
	}
}

