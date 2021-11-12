// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon.h"
#include "WeaponHandGun.generated.h"

/**
 * 
 */
UCLASS()
class TENCENT3_API AWeaponHandGun : public AWeapon
{
	GENERATED_BODY()
public:
	virtual void SetFire(bool Value) override;
};
