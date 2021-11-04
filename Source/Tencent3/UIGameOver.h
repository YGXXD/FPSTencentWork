// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UIRecordList.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "UIGameOver.generated.h"

/**
 * 
 */
UCLASS()
class TENCENT3_API UUIGameOver : public UUserWidget
{
	GENERATED_BODY()

private:
	UPROPERTY()
	UTextBlock* ScoreTextBlock;

	UPROPERTY()
	UButton* BackMainMenuButton;

	UPROPERTY()
	UButton* LookRecordButton;
	
	FScriptDelegate BackMainMenuButtonDelegate;
	
	FScriptDelegate RePlayButtonDelegate;
protected:
	virtual bool Initialize() override;

public:
	UFUNCTION()
	void SetScoreText(int value);

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Init")
	TSubclassOf<UUIRecordList> PlayerInfoRecordClass;
	
private:
	UFUNCTION()
	void LookRecord();

	UFUNCTION()
	void BackMainMenu();
};
