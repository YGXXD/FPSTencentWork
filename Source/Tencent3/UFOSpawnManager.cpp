// Fill out your copyright notice in the Description page of Project Settings.


#include "UFOSpawnManager.h"

// Sets default values
AUFOSpawnManager::AUFOSpawnManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AUFOSpawnManager::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AUFOSpawnManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	Time += GetWorld()->DeltaTimeSeconds;
	if(Time > SpawnInterval)
	{
		Time = 0;
		UWorld* World = GetWorld();
		if (World)
		{
			AEnemyUFO* UFO = World->SpawnActor<AEnemyUFO>(UFOClass,this->GetTransform());
		}
	}
	
}

