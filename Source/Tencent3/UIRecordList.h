// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/ScrollBox.h"
#include "Components/TextBlock.h"
#include "UIRecordList.generated.h"

/**
 * 
 */
UCLASS()
class TENCENT3_API UUIRecordList : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Init")
	TSubclassOf<UUserWidget> PlayerInfoRecordClass;

	UPROPERTY()
	UTextBlock* PlayerName;

	UPROPERTY()
	UTextBlock* PlayerScore;

	UPROPERTY()
	UScrollBox* ScrollBox;

	UPROPERTY()
	UButton* BackMainMenuButton;

	//返回大厅委托
	FScriptDelegate BackMainMenuButtonDelegate;
private:
	virtual bool Initialize() override;

	UFUNCTION()
	void BackMainMenu();
	
};
