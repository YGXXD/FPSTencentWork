// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponHandGun.h"

void AWeaponHandGun::SetFire(bool Value)
{
	Super::SetFire(Value);
	if(bIsFire == true)
	{
		Fire();
	}
}
