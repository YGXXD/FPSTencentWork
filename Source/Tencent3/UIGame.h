// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "UIGame.generated.h"

/**
 * 
 */
UCLASS()
class TENCENT3_API UUIGame : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual bool Initialize() override;
private:
	UPROPERTY()
	UTextBlock* ScoreTextBlock;

	UPROPERTY()
	UTextBlock* TimeTextBlock;

	UPROPERTY()
	UTextBlock* PauseTextBlock;

	UPROPERTY()
	UTextBlock* GameStateTextBlock;

	UPROPERTY()
	UTextBlock* PlayerNameTextBlock;
	
	UPROPERTY()
	UButton* PauseButton;
	
	UPROPERTY()
	UButton* FireButton;

	UPROPERTY()
	UButton* JumpButton;
	//暂停按钮委托
	FScriptDelegate PauseButtonDelegate;

	//开火按钮委托
	FScriptDelegate FireButtonDelegate;

	//跳约按钮委托
	FScriptDelegate JumpButtonDelegate;
	UPROPERTY()
	bool bIsPause = false;
public:
	UFUNCTION()
	void UpdateScore(int value);

	UFUNCTION()
	void UpdateTime(int value);

	UFUNCTION()
	void Pause();
};
