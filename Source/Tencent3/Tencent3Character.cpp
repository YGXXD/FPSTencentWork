// Copyright Epic Games, Inc. All Rights Reserved.

#include "Tencent3Character.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

//////////////////////////////////////////////////////////////////////////
// ATencent3Character

ATencent3Character::ATencent3Character()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
	
	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)

	PlayerMesh = GetMesh();
	PlayerMovementComponent = Cast<UCharacterMovementComponent>(GetMovementComponent());

	//载入武器槽位
	PlayerWeapon.Add(nullptr);
	PlayerWeapon.Add(nullptr);
	PlayerWeapon.Add(nullptr);

	//载入WeaponOperation的数据
	FWeaponOperationData PlayerWeaponOperationData;
	PlayerWeaponOperationData.BackSkeletalSocket = TEXT("BackGunSocket1");
	PlayerWeaponOperationData.HandSkeletalSocket = TEXT("Hand_rSocket1");
	PlayerWeaponOperationData.ReleaseWeaponMontage = LoadObject<UAnimMontage>(this,TEXT("AnimMontage'/Game/MyFile/Anim/CharacterAnimation/TakeWeapon/ReleaseWeapon.ReleaseWeapon'"));
	PlayerWeaponOperationData.TakeWeaponMontage = LoadObject<UAnimMontage>(this,TEXT("AnimMontage'/Game/MyFile/Anim/CharacterAnimation/TakeWeapon/TakeWeapon.TakeWeapon'"));
	WeaponOperations.Add(PlayerWeaponOperationData);

	PlayerWeaponOperationData.BackSkeletalSocket = TEXT("BackGunSocket2");
	PlayerWeaponOperationData.HandSkeletalSocket = TEXT("Hand_lSocket");
	PlayerWeaponOperationData.ReleaseWeaponMontage = LoadObject<UAnimMontage>(this,TEXT("AnimMontage'/Game/MyFile/Anim/CharacterAnimation/TakeWeapon/ReleaseWeaponMotage2.ReleaseWeaponMotage2'"));
	PlayerWeaponOperationData.TakeWeaponMontage = LoadObject<UAnimMontage>(this,TEXT("AnimMontage'/Game/MyFile/Anim/CharacterAnimation/TakeWeapon/TakeWeaponMontage2.TakeWeaponMontage2'"));
	WeaponOperations.Add(PlayerWeaponOperationData);

	PlayerWeaponOperationData.BackSkeletalSocket = TEXT("ThighRSocket");
	PlayerWeaponOperationData.HandSkeletalSocket = TEXT("Hand_rSocket2");
	PlayerWeaponOperationData.ReleaseWeaponMontage = LoadObject<UAnimMontage>(this,TEXT("AnimMontage'/Game/MyFile/Anim/CharacterAnimation/TakeWeapon/ReleaseHandGunMontage.ReleaseHandGunMontage'"));
	PlayerWeaponOperationData.TakeWeaponMontage = LoadObject<UAnimMontage>(this,TEXT("AnimMontage'/Game/MyFile/Anim/CharacterAnimation/TakeWeapon/TakeHandGunMontage.TakeHandGunMontage'"));
	WeaponOperations.Add(PlayerWeaponOperationData);

	StandToCrouchMontage = LoadObject<UAnimMontage>(this,TEXT("AnimMontage'/Game/MyFile/Anim/CharacterAnimation/Montage/StandToCrouchMontage.StandToCrouchMontage'"));
	CrouchToStandMontage = LoadObject<UAnimMontage>(this,TEXT("AnimMontage'/Game/MyFile/Anim/CharacterAnimation/Montage/CrouchToStandMontage.CrouchToStandMontage'"));
}

void ATencent3Character::BeginPlay()
{
	Super::BeginPlay();
	GetWeapon();
}

void ATencent3Character::Tick(float DeltaSeconds)
{
	//FRotator PlayerActorRotator = GetActorRotation();
	//FRotator PlayerAimRotator = GetBaseAimRotation();
	Super::Tick(DeltaSeconds);
	FRotator DeltaRotator = UKismetMathLibrary::NormalizedDeltaRotator(GetBaseAimRotation(),GetActorRotation());
	
	AimPitch = DeltaRotator.Pitch;
	AimYaw = DeltaRotator.Yaw;
	
	if(bIsAim)
	{
		CameraBoom->TargetArmLength = FMath::Lerp(CameraBoom->TargetArmLength,150.0f,0.1);
		CameraBoom->SocketOffset = FMath::Lerp(CameraBoom->SocketOffset,FVector(0,100,60),0.1);
	}
	else
	{
		CameraBoom->TargetArmLength = FMath::Lerp(CameraBoom->TargetArmLength,300.0f,0.1);
		CameraBoom->SocketOffset = FMath::Lerp(CameraBoom->SocketOffset,FVector(0,0,60),0.1);
	}
}

//////////////////////////////////////////////////////////////////////////
// Input

void ATencent3Character::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	PlayerInputComponent->BindAction("Attack",IE_Pressed,this,&ATencent3Character::Attack);//绑定攻击
	PlayerInputComponent->BindAction("Attack",IE_Released,this,&ATencent3Character::CancelAttack);//绑定攻击
	PlayerInputComponent->BindAction("Weapon",IE_Pressed,this,&ATencent3Character::Weapon);//绑定收枪拔枪
	PlayerInputComponent->BindAction("Aim",IE_Pressed,this,&ATencent3Character::EnterAim);//绑定瞄准
	PlayerInputComponent->BindAction("Aim",IE_Released,this,&ATencent3Character::ExitAim);//绑定退出瞄准
	PlayerInputComponent->BindAction("ChangeMagazine",IE_Released,this,&ATencent3Character::ChangeMagazine);
	PlayerInputComponent->BindAction("Crouch",IE_Pressed,this,&ATencent3Character::CrouchControl);
	PlayerInputComponent->BindAction("ChangeWeapon1",IE_Pressed,this,&ATencent3Character::ChangeWeaponOne);
	PlayerInputComponent->BindAction("ChangeWeapon2",IE_Pressed,this,&ATencent3Character::ChangeWeaponTwo);
	PlayerInputComponent->BindAction("ChangeWeapon3",IE_Pressed,this,&ATencent3Character::ChangeWeaponThree);
	PlayerInputComponent->BindAction("LockDirection",IE_Pressed,this,&ATencent3Character::LockDirection);//绑定瞄准
	PlayerInputComponent->BindAction("LockDirection",IE_Released,this,&ATencent3Character::UnLockDirection);//绑定退出瞄准
	

	
	PlayerInputComponent->BindAxis("MoveForward", this, &ATencent3Character::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ATencent3Character::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &ATencent3Character::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &ATencent3Character::LookUpAtRate);
	
}

void ATencent3Character::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ATencent3Character::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void ATencent3Character::MoveForward(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		FRotator Rotation;
		if(bIsLockDirection)
		{
			Rotation =  PlayerMesh->GetComponentRotation();
			Rotation.Yaw += 90;
		}
		// find out which way is forward
		else
		{
			Rotation = Controller->GetControlRotation();
		}
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void ATencent3Character::MoveRight(float Value)
{
	if ( (Controller != nullptr) && (Value != 0.0f) )
	{
		FRotator Rotation;
		if(bIsLockDirection)
		{
			Rotation =  PlayerMesh->GetComponentRotation();
			Rotation.Yaw += 90;
		}
		// find out which way is forward
		else
		{
			Rotation = Controller->GetControlRotation();
		}
		// find out which way is right
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

void ATencent3Character::Attack()
{
	if(bIsTakeWeapon)
	{
		PlayerWeapon[WeaponIndex]->SetFire(true);
	}
}

void ATencent3Character::CancelAttack()
{
	PlayerWeapon[WeaponIndex]->SetFire(false);;
}


void ATencent3Character::EnterAim()
{
	if(bIsTakeWeapon)
	{
		bUseControllerRotationYaw = true;
		bIsAim = true;
		PlayerWeapon[WeaponIndex]->Offset.Y = 85;
		if(bIsCrouch)
		{
			PlayerMovementComponent->MaxWalkSpeed = 120;
		}
		else
		{
			PlayerMovementComponent->MaxWalkSpeed = 200;
		}
	}
}

void ATencent3Character::ExitAim()
{
	bIsAim = false;
	bUseControllerRotationYaw = false;
	PlayerWeapon[WeaponIndex]->Offset.Y = 0;
	if(bIsCrouch)
	{
		PlayerMovementComponent->MaxWalkSpeed = 250;
	}
	else
	{
		PlayerMovementComponent->MaxWalkSpeed = 600;
	}
}

//子弹射击逻辑在Weapon类中已经实现
/*void ATencent3Character::Fire()
{
	//限制开火间隔
	if(GetWorld()->GetTimeSeconds() > Time)
	{
		if(bIsTakeWeapon)
		{
			USoundCue* ShootSound = LoadObject<USoundCue>(this,TEXT("SoundCue'/Game/MyFile/Sound/Shoot_Cue.Shoot_Cue'"));
			if(ShootSound)
			{
				UGameplayStatics::PlaySoundAtLocation(this,ShootSound,GetMesh()->GetComponentLocation());
			}
			PlayAnimMontage(FireMontage);
			UAnimInstance* WeaponAnim = PlayerWeapon->WeaponMesh->GetAnimInstance();
			if(WeaponAnim)
			{
				WeaponAnim->Montage_Play(PlayerWeapon->FireMontage);
			}
			//试图发射发射物。
			if (ProjectileClass)
			{
				// 获取摄像机变换。
				FVector CameraLocation;
				FRotator CameraRotation;
				GetActorEyesViewPoint(CameraLocation, CameraRotation);
		

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
					AProjectile* Projectile = World->SpawnActor<AProjectile>(ProjectileClass, MuzzleLocation, MuzzleRotation, SpawnParams);
					if (Projectile)
					{
						// 设置发射物的初始轨迹。
						FVector LaunchDirection = MuzzleRotation.Vector();
						Projectile->FireInDirection(LaunchDirection);
					}
				}
			}
		}
		Time = GetWorld()->GetTimeSeconds() + FireInterval;
	}
}*/

void ATencent3Character::GetWeapon()
{
	UWorld* World = GetWorld();
	if(World)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = GetInstigator();
		
		AWeapon* Weapon = World->SpawnActor<AWeapon>(BaseWeapon,SpawnParams);
		Weapon->K2_AttachToComponent(PlayerMesh,WeaponOperations[0].BackSkeletalSocket,
			EAttachmentRule::SnapToTarget,EAttachmentRule::SnapToTarget,
			EAttachmentRule::SnapToTarget,true);
		PlayerWeapon[0] = Weapon;

		Weapon = World->SpawnActor<AWeapon>(BaseWeapon,SpawnParams);
		Weapon->K2_AttachToComponent(PlayerMesh,WeaponOperations[1].BackSkeletalSocket,
			EAttachmentRule::SnapToTarget,EAttachmentRule::SnapToTarget,
			EAttachmentRule::SnapToTarget,true);
		PlayerWeapon[1] = Weapon;

		Weapon = World->SpawnActor<AWeapon>(HandGunWeapon,SpawnParams);
		Weapon->K2_AttachToComponent(PlayerMesh,WeaponOperations[2].BackSkeletalSocket,
			EAttachmentRule::SnapToTarget,EAttachmentRule::SnapToTarget,
			EAttachmentRule::SnapToTarget,true);
		PlayerWeapon[2] = Weapon;

		PlayerWeapon[WeaponIndex]->UpdateUIBulletNum();
	}
}

void ATencent3Character::Weapon()
{
	if(bIsCombatReady)
	{
		ReleaseWeapon();
	}
	else
	{
		TakeWeapon();
	}
}

void ATencent3Character::TakeWeapon()
{
	PlayAnimMontage(WeaponOperations[WeaponIndex].TakeWeaponMontage);
	bIsCombatReady = true;
}

void ATencent3Character::IsWeaponToTrue()
{
	bIsTakeWeapon=true;
	switch (WeaponIndex)
	{
	case 0:
		break;
	case 1:
		PlayerWeapon[WeaponIndex]->K2_AttachToComponent(PlayerMesh,WeaponOperations[0].HandSkeletalSocket,
			EAttachmentRule::SnapToTarget,EAttachmentRule::SnapToTarget,
			EAttachmentRule::SnapToTarget,true);
		break;
	case 2:
		break;
	default:
		break;
	}
}

void ATencent3Character::WeaponToHand()
{
	PlayerWeapon[WeaponIndex]->K2_AttachToComponent(PlayerMesh,WeaponOperations[WeaponIndex].HandSkeletalSocket,
			EAttachmentRule::SnapToTarget,EAttachmentRule::SnapToTarget,
			EAttachmentRule::SnapToTarget,true);
}

void ATencent3Character::ReleaseWeapon()
{
	IsWeaponToFalse();
	switch (WeaponIndex)
	{
	case 0:
		break;
	case 1:
		PlayerWeapon[WeaponIndex]->K2_AttachToComponent(PlayerMesh,WeaponOperations[WeaponIndex].HandSkeletalSocket,
			EAttachmentRule::SnapToTarget,EAttachmentRule::SnapToTarget,
			EAttachmentRule::SnapToTarget,true);
		break;
	case 2:
		break;
	default:
		break;
	}
	CancelAttack();
	PlayAnimMontage(WeaponOperations[WeaponIndex].ReleaseWeaponMontage);
	bIsCombatReady = false;
	if(bIsAim)
		ExitAim();
}

void ATencent3Character::WeaponToBack()
{
	PlayerWeapon[WeaponIndex]->K2_AttachToComponent(PlayerMesh,WeaponOperations[WeaponIndex].BackSkeletalSocket,
			EAttachmentRule::SnapToTarget,EAttachmentRule::SnapToTarget,
			EAttachmentRule::SnapToTarget,true);
}

void ATencent3Character::IsWeaponToFalse()
{
	bIsTakeWeapon=false;
}

void ATencent3Character::ChangeMagazine()
{
	if(!PlayerWeapon[WeaponIndex]->AllBulletNum)
	{
		return;
	}
	if(bIsTakeWeapon)
	{
		PlayerWeapon[WeaponIndex]->ChangeMagazine();
		bIsTakeWeapon = false;
		if(bIsAim)
		{
			ExitAim();
		}
	}
}

void ATencent3Character::ChangeWeapon(int Index)
{
	WeaponIndex = Index;
	PlayerWeapon[WeaponIndex]->UpdateUIBulletNum();
}
void ATencent3Character::ChangeWeaponOne()
{
	ChangeWeapon(0);
}

void ATencent3Character::ChangeWeaponTwo()
{
	ChangeWeapon(1);
}

void ATencent3Character::ChangeWeaponThree()
{
	ChangeWeapon(2);
}


void ATencent3Character::CrouchControl()
{
	if(bIsCrouch)
	{
		CrouchToStand();
	}
	else
	{
		StandToCrouch();
	}
}


void ATencent3Character::CrouchToStand()
{
	bIsCrouch = false;
	PlayAnimMontage(CrouchToStandMontage);
	PlayerMesh->SetRelativeLocation(FVector(0,0,-97));
	if(bIsAim)
		PlayerMovementComponent->MaxWalkSpeed = 200;
	else
		PlayerMovementComponent->MaxWalkSpeed = 600;
}

void ATencent3Character::StandToCrouch()
{
	bIsCrouch = true;
	PlayAnimMontage(StandToCrouchMontage);
	PlayerMesh->SetRelativeLocation(FVector(0,0,-110));//蹲下会往上偏移调整坐标
	if(bIsAim)
		PlayerMovementComponent->MaxWalkSpeed = 120;
	else
		PlayerMovementComponent->MaxWalkSpeed = 250;
}

void ATencent3Character::LockDirection()
{
	bIsLockDirection = true;
	PlayerMovementComponent->bOrientRotationToMovement = false;
}

void ATencent3Character::UnLockDirection()
{
	bIsLockDirection = false;
	PlayerMovementComponent->bOrientRotationToMovement = true;
}

