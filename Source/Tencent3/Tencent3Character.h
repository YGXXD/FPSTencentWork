// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Weapon.h"
#include "GameFramework/Character.h"
#include "Tencent3Character.generated.h"


USTRUCT()
struct FWeaponOperationData
{
	GENERATED_BODY()
	FName BackSkeletalSocket;
	FName HandSkeletalSocket;
	UPROPERTY()
	UAnimMontage* ReleaseWeaponMontage;
	UPROPERTY()
	UAnimMontage* TakeWeaponMontage;
};

UCLASS(config=Game)
class ATencent3Character : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	UPROPERTY()
	USkeletalMeshComponent* PlayerMesh;//获取Player的Mesh

	UPROPERTY()
	UCharacterMovementComponent* PlayerMovementComponent;//获取移动组件

public:
	ATencent3Character();

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Init")
	TSubclassOf<class AWeapon> BaseWeapon;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Init")
	TSubclassOf<class AWeapon> HandGunWeapon;

	UPROPERTY()
	TArray<AWeapon*> PlayerWeapon;

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
	int WeaponIndex = 0;

	//蒙太奇动画
	UPROPERTY()
	UAnimMontage* StandToCrouchMontage;
	
	UPROPERTY()
	UAnimMontage* CrouchToStandMontage;
	
	UPROPERTY()
	TArray<FWeaponOperationData> WeaponOperations;

protected:
	
	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/** 
	 * Called via input to turn at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

private:
	UFUNCTION()
	void GetWeapon();

	//拿出武器和放回武器
	UFUNCTION()
	void TakeWeapon();

	UFUNCTION()
	void ReleaseWeapon();
	
	//瞄准
	UFUNCTION()
    void EnterAim();

	UFUNCTION()
	void ExitAim();

	//起立和蹲下
	UFUNCTION()
	void StandToCrouch();

	UFUNCTION()
	void CrouchToStand();

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;
public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	//攻击和取消攻击
	UFUNCTION()
	void Attack();

	UFUNCTION()
	void CancelAttack();
	
	UFUNCTION()
	void Weapon();

	//动画通知调用
	UFUNCTION(BlueprintCallable)
	void WeaponToHand();

	UFUNCTION(BlueprintCallable)
	void WeaponToBack();

	//动画通知调用
	UFUNCTION(BlueprintCallable)
	void IsWeaponToTrue();

	UFUNCTION(BlueprintCallable)
	void IsWeaponToFalse();

	UFUNCTION()
	void ChangeMagazine();//换子弹
	
	UFUNCTION()
	void CrouchControl();

	//切换武器
	UFUNCTION()
	void ChangeWeapon(int Index);
	
	//切换武器的输入执行
	UFUNCTION()
	void ChangeWeaponOne();

	UFUNCTION()
	void ChangeWeaponTwo();
	
	UFUNCTION()
	void ChangeWeaponThree();

	//Alt锁角色方向
	UFUNCTION()
	void LockDirection();
	
	UFUNCTION()
	void UnLockDirection();
/////////////////////////////////////////////////////////////////////////////////////////////////////////
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
	bool bIsTakeWeapon = false;//是否拿着武器

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
	bool bIsCombatReady = false;//是否装备准备

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
	bool bIsAim = false;//是否瞄准

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
	bool bIsCrouch = false;//是否蹲下

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
	bool bIsLockDirection = false;//是否锁视角
	
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
	float AimYaw;//瞄准偏移

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
	float AimPitch;//瞄准偏移
	
};

