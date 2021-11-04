// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyUFO.h"
#include "GameFramework/Actor.h"
#include "UFOSpawnManager.generated.h"

UCLASS()
class TENCENT3_API AUFOSpawnManager : public AActor
{
	GENERATED_BODY()
private:
	float Time = 0;
	
public:	
	// Sets default values for this actor's properties
	AUFOSpawnManager();

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Init")
	float SpawnInterval = 3.0f;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Init")
	TSubclassOf<AEnemyUFO> UFOClass;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
