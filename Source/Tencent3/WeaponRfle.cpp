// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponRfle.h"

#include "Tencent3Character.h"
#include "GameFramework/Character.h"

void AWeaponRfle::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if(bIsFire)
	{
		Fire();
	}
}

void AWeaponRfle::Fire()
{
	if(GetWorld()->GetTimeSeconds() > Time)
	{
		Super::Fire();
		Time = GetWorld()->GetTimeSeconds() + FireInterval;
	}
}