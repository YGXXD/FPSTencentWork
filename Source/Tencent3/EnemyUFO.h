// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "PhysicsEngine/PhysicsThrusterComponent.h"
#include "EnemyUFO.generated.h"

UCLASS()
class TENCENT3_API AEnemyUFO : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemyUFO();

	UPROPERTY(VisibleAnywhere)
	UBoxComponent* ColliderUFO;
	
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* StaticMeshUFO;

	UPROPERTY(VisibleAnywhere)
	UPhysicsThrusterComponent* ThrusterUFO;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Init")
	float ThrusterSpeed = 300000.0f;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Init")
	float RotatorSpeed = 1000.0f;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit) override;
};
