// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon.h"
#include "WeaponRfle.generated.h"

/**
 * 
 */
UCLASS()
class TENCENT3_API AWeaponRfle : public AWeapon
{
	GENERATED_BODY()

	UPROPERTY()
	float Time = 0;

protected:
	virtual void Tick(float DeltaSeconds) override;

	virtual void Fire() override;

public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Init")
	float FireInterval = 0.1f;//开火间隔
};
