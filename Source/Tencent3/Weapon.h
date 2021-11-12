// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Projectile.h"
#include "UIGame.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

class ATencent3Character;
UCLASS()
class TENCENT3_API AWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeapon();

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
	USkeletalMeshComponent* WeaponMesh;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Anim")
	UAnimMontage* FireMontage;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Anim")
	UAnimMontage* PlayerFireMontage;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Anim")
	UAnimMontage* ChangeMagazineMontage;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Anim")
	UAnimMontage* PlayerChangeMagazineMontage;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY()
	bool bIsFire = false;

	UPROPERTY()
	ATencent3Character* Player;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	virtual void SetFire(bool Value);

	UFUNCTION()
	virtual void Fire();

	UFUNCTION()
	virtual void ChangeMagazine();

	UFUNCTION()
	void UpdateUIBulletNum();
	//////////////////////////////////////////////////////////////////////

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Init")
	FVector Offset;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Init")
	TSubclassOf<AProjectile> ProjectileClass;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Init")
	int AllBulletNum = 90;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Init")
	int MagazineBulletNum = 30;

	UPROPERTY()
	int CurrentMagazineBulletNum;

	UPROPERTY()
	UUIGame* GameWidget;
};
