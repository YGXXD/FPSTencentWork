// Fill out your copyright notice in the Description page of Project Settings.


#include "UIRecordList.h"

#include <string>

#include "MyGameInstance.h"
#include "Kismet/GameplayStatics.h"

bool UUIRecordList::Initialize()
{
	Super::Initialize();
	ScrollBox = Cast<UScrollBox>(GetWidgetFromName("PlayerListScrollBox"));
	BackMainMenuButton = Cast<UButton>(GetWidgetFromName("BackMainMenu"));
	UMyGameInstance* GameInstance = Cast<UMyGameInstance>(GetGameInstance());
	TArray<FPlayerInfo> PlayerInfoList;
	if(GameInstance && ScrollBox)
	{
		//查询战绩，将记录添加到滚动框子项
		GameInstance->LoadPlayerInfo();
		PlayerInfoList = GameInstance->PlayerInfoList;
		for(int i = PlayerInfoList.Num() - 1; i >= 0; i--)
		{
			UUserWidget* NewPlayerInfoWidget = CreateWidget<UUserWidget>(GetWorld(),PlayerInfoRecordClass);
			ScrollBox->AddChild(NewPlayerInfoWidget);
			PlayerName = Cast<UTextBlock>(NewPlayerInfoWidget->GetWidgetFromName("Name"));
			PlayerScore = Cast<UTextBlock>(NewPlayerInfoWidget->GetWidgetFromName("Score"));
			PlayerName->SetText(FText::FromString(PlayerInfoList[i].Name));
			PlayerScore->SetText(FText::FromString(FString::FromInt(PlayerInfoList[i].Score)));
		}
	}

	//返回大厅绑定
	if(BackMainMenuButton)
	{
		BackMainMenuButtonDelegate.BindUFunction(this,"BackMainMenu");
		BackMainMenuButton->OnClicked.Add(BackMainMenuButtonDelegate);
	}
	return true;
}

void UUIRecordList::BackMainMenu()
{
	UMyGameInstance* GameInstance = Cast<UMyGameInstance>(GetGameInstance());
	if(GameInstance)
	{
		GameInstance->PlayerInfoList.Empty();
	}
	UGameplayStatics::OpenLevel(GetWorld(),TEXT("Map_MainMenu"));
}
