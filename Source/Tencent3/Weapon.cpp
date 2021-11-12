// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"

#include "Projectile.h"
#include "Tencent3Character.h"
#include "Tencent3GameMode.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"

// Sets default values
AWeapon::AWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponSkeletal"));
	
}

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	Player =  Cast<ATencent3Character>(UGameplayStatics::GetPlayerCharacter(GetWorld(),0));
	CurrentMagazineBulletNum = MagazineBulletNum;
	GameWidget = Cast<ATencent3GameMode>(UGameplayStatics::GetGameMode(GetWorld()))->GameWidget;
}

// Called every frame
void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AWeapon::SetFire(bool Value)
{
	bIsFire = Value;
}

void AWeapon::Fire()
{
	if(!CurrentMagazineBulletNum)
	{
		return;
	}
	CurrentMagazineBulletNum--;
	USoundCue* ShootSound = LoadObject<USoundCue>(this,TEXT("SoundCue'/Game/MyFile/Sound/Shoot_Cue.Shoot_Cue'"));
	if (ShootSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, ShootSound, WeaponMesh->GetComponentLocation());
	}
	UAnimInstance* WeaponAnim = WeaponMesh->GetAnimInstance();
	if (WeaponAnim)
	{
		WeaponAnim->Montage_Play(FireMontage);
	}
	if(PlayerFireMontage)
		Player->PlayAnimMontage(PlayerFireMontage);

	UpdateUIBulletNum();
	
	//试图发射发射物。
	if (ProjectileClass)
	{
		// 获取摄像机变换。
		FVector CameraLocation;
		FRotator CameraRotation;
		Player->GetActorEyesViewPoint(CameraLocation, CameraRotation);


		// 将MuzzleOffset从摄像机空间变换到世界空间。
		FVector MuzzleLocation = CameraLocation + FTransform(CameraRotation).TransformVector(Offset);

		// 使目标方向略向上倾斜。
		FRotator MuzzleRotation = CameraRotation;
		MuzzleRotation.Pitch += 0.5f;

		UWorld* World = GetWorld();
		if (World)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.Instigator = GetInstigator();

			// 在枪口位置生成发射物。
			AProjectile* Projectile = World->SpawnActor<AProjectile>(ProjectileClass, MuzzleLocation, MuzzleRotation,
			                                                         SpawnParams);
			if (Projectile)
			{
				// 设置发射物的初始轨迹。
				FVector LaunchDirection = MuzzleRotation.Vector();
				Projectile->FireInDirection(LaunchDirection);
			}
		}
	}
}

void AWeapon::UpdateUIBulletNum()
{
	if(GameWidget)
	{
		GameWidget->UpdateBulletNum(CurrentMagazineBulletNum,AllBulletNum);
	}
}


void AWeapon::ChangeMagazine()
{
	UAnimInstance* WeaponAnim = WeaponMesh->GetAnimInstance();
	if(WeaponAnim)
	{
		WeaponAnim->Montage_Play(ChangeMagazineMontage);
	}
	Player->PlayAnimMontage(PlayerChangeMagazineMontage);
	
	const int DeltaBulletNum = MagazineBulletNum - CurrentMagazineBulletNum; 
	CurrentMagazineBulletNum = MagazineBulletNum;
	AllBulletNum -= DeltaBulletNum;
	if(AllBulletNum < 0)
	{
		CurrentMagazineBulletNum += AllBulletNum;
		AllBulletNum = 0;
	}
	UpdateUIBulletNum();
}
