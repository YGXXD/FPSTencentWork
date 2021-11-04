// Fill out your copyright notice in the Description page of Project Settings.


#include "UIGameOver.h"

#include "Kismet/GameplayStatics.h"

bool UUIGameOver::Initialize()
{
	Super::Initialize();
	ScoreTextBlock = Cast<UTextBlock>(GetWidgetFromName("Score"));
	BackMainMenuButton = Cast<UButton>(GetWidgetFromName("BackMainMenu"));
	LookRecordButton = Cast<UButton>(GetWidgetFromName("LookRecord"));
	if(BackMainMenuButton && LookRecordButton)
	{
		BackMainMenuButtonDelegate.BindUFunction(this,"BackMainMenu");
		RePlayButtonDelegate.BindUFunction(this,"LookRecord");
		BackMainMenuButton->OnClicked.Add(BackMainMenuButtonDelegate);
		LookRecordButton->OnClicked.Add(RePlayButtonDelegate);
	}
	return true;
}

void UUIGameOver::SetScoreText(int value)
{
	if(ScoreTextBlock)
		ScoreTextBlock->SetText(FText::FromString(FString::FromInt(value)));
}

void UUIGameOver::LookRecord()
{
	RemoveFromParent();
	UUserWidget* NewPlayerInfoWidget = CreateWidget<UUserWidget>(GetWorld(),PlayerInfoRecordClass);
	NewPlayerInfoWidget->AddToViewport();
}

void UUIGameOver::BackMainMenu()
{
	UGameplayStatics::OpenLevel(GetWorld(),TEXT("Map_MainMenu"));
}


