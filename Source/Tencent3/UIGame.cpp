// Fill out your copyright notice in the Description page of Project Settings.


#include "UIGame.h"


#include "MyGameInstance.h"
#include "Tencent3Character.h"
#include "Kismet/GameplayStatics.h"

bool UUIGame::Initialize()
{
	Super::Initialize();
	ScoreTextBlock = Cast<UTextBlock>(GetWidgetFromName("Score"));
	TimeTextBlock = Cast<UTextBlock>(GetWidgetFromName("Time"));
	PauseTextBlock = Cast<UTextBlock>(GetWidgetFromName("PauseText"));
	GameStateTextBlock = Cast<UTextBlock>(GetWidgetFromName("GameState"));
	PlayerNameTextBlock = Cast<UTextBlock>(GetWidgetFromName("PlayerName"));
	CurrentBulletTextBlock = Cast<UTextBlock>(GetWidgetFromName("CurrentBullet"));
	AllBulletTextBlock = Cast<UTextBlock>(GetWidgetFromName("AllBullet"));
	PauseButton = Cast<UButton>(GetWidgetFromName("Pause"));
	FireButton = Cast<UButton>(GetWidgetFromName("Fire"));
	JumpButton = Cast<UButton>(GetWidgetFromName("Jump"));
	
	if(PauseButton)
	{
		GameStateTextBlock->SetText(FText::FromName(TEXT("游戏中")));
		PauseButtonDelegate.BindUFunction(this,"Pause");
		PauseButton->OnClicked.Add(PauseButtonDelegate);
	}

	if(FireButton && JumpButton)
	{
		ATencent3Character* Player = Cast<ATencent3Character>(UGameplayStatics::GetPlayerCharacter(this,0));
		if(Player)
		{
			FireButtonDelegate.BindUFunction(Player,"Attack");
			FireButton->OnClicked.Add(FireButtonDelegate);

			JumpButtonDelegate.BindUFunction(Player,"Jump");
			JumpButton->OnClicked.Add(JumpButtonDelegate);
		}
	}

	UMyGameInstance* GameInstance = Cast<UMyGameInstance>(GetGameInstance());
	if(GameInstance)
	{
		PlayerNameTextBlock->SetText(FText::FromString(GameInstance->PlayerInfo.Name));	
	}
	
	return true;
}

void UUIGame::UpdateScore(int value)
{
	ScoreTextBlock->SetText(FText::FromString(FString::FromInt(value)));
}
void UUIGame::UpdateTime(int value)
{
	TimeTextBlock->SetText(FText::FromString(FString::FromInt(value)));
}

void UUIGame::Pause()
{
	if(bIsPause)
	{
		bIsPause = !bIsPause;
		PauseTextBlock->SetText(FText::FromString(TEXT("暂停游戏")));
		GameStateTextBlock->SetText(FText::FromName(TEXT("游戏中")));
		UGameplayStatics::SetGlobalTimeDilation(GetWorld(),1.0f);
	}
	else
	{
		bIsPause = !bIsPause;
		PauseTextBlock->SetText(FText::FromString(TEXT("继续游戏")));
		GameStateTextBlock->SetText(FText::FromName(TEXT("暂停中")));
		UGameplayStatics::SetGlobalTimeDilation(GetWorld(),0.0f);
	}
}

void UUIGame::UpdateBulletNum(int CurrentBulletNum, int AllBulletNum)
{
	CurrentBulletTextBlock->SetText(FText::FromString(FString::FromInt(CurrentBulletNum)));
	AllBulletTextBlock->SetText(FText::FromString(FString::FromInt(AllBulletNum)));
}

