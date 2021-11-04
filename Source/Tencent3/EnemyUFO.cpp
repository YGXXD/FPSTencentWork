// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyUFO.h"

#include "Tencent3GameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"


// Sets default values
AEnemyUFO::AEnemyUFO()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	ColliderUFO = CreateDefaultSubobject<UBoxComponent>(TEXT("ColliderBox"));
	ColliderUFO->SetSimulatePhysics(true);
	ColliderUFO->SetEnableGravity(false);
	ColliderUFO->BodyInstance.bLockYRotation = true;
	ColliderUFO->BodyInstance.bLockXRotation = true;
	ColliderUFO->SetGenerateOverlapEvents(true);
	RootComponent = ColliderUFO;
	
	StaticMeshUFO = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshUFO"));
	StaticMeshUFO->SetupAttachment(ColliderUFO);
	
	ThrusterUFO = CreateDefaultSubobject<UPhysicsThrusterComponent>(TEXT("Thruster"));
	ThrusterUFO->SetupAttachment(RootComponent);
	ThrusterUFO->SetAutoActivate(true);
	ThrusterUFO->SetWorldRotation(UKismetMathLibrary::MakeRotFromX(this->GetActorForwardVector()));
	ThrusterUFO->ThrustStrength = ThrusterSpeed;
}

// Called when the game starts or when spawned
void AEnemyUFO::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEnemyUFO::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if(ThrusterSpeed>0)
	{
		ThrusterSpeed -= 1500.0f;
		ThrusterUFO->ThrustStrength = ThrusterSpeed;
	}
	else
	{
		ThrusterUFO->ThrustStrength = 0;
	}
	StaticMeshUFO->AddRelativeRotation(FRotator(0,GetWorld()->DeltaTimeSeconds*RotatorSpeed,0));
}

void AEnemyUFO::NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
	if(Other->GetOwner())
	{
		ATencent3GameMode* GameMode = GetWorld()->GetAuthGameMode<ATencent3GameMode>();
		GameMode->AddScore(10);
		Other->Destroy();
		this->Destroy();
	}
}




