// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Equipment/EquipableItem.h"
#include "Components/Weapon/WeaponBarellComponent.h"
#include "RangeWeaponItem.generated.h"

/**
 *
 */

DECLARE_MULTICAST_DELEGATE(FOnReloadComplete);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnAmmoChanged, int32);

UENUM(BlueprintType)
enum class EWeaponFireMode : uint8
{
	Single,
	FullAuto
};

UENUM(BlueprintType)
enum class EReloadType : uint8
{
	FullClip,
	ByBullet
};

class UAnimMontage;
UCLASS(Blueprintable)
class HOMEWORKPROJECT_API ARangeWeaponItem : public AEquipableItem
{
	GENERATED_BODY()

public:
	ARangeWeaponItem();

	void StartFire();

	void StopFire();

	void StartAim();

	void StopAim();

	bool IsFiring() const;

	bool IsReloading() const;

	void StartReload();

	void EndReload(bool bIsSuccess);

	float GetAimFOV() const;

	float GetAimMovementMaxSpeed() const;

	FTransform GetForeGripTransform() const;

	float GetAimTurnModifier() { return AimingTurnModifier; }

	float GetAimLookUpModifier() { return AimimgLookUpModifier; }

	int32 GetAmmo() const;

	int32 GetMaxAmmo() const;

	int32 GetOptionalAmmo() const;

	int32 GetMaxOptionalAmmo() const;

	void SetAmmo(int32 NewAmmo);

	void SetOptionalAmmo(int32 NewAmmo);

	EAmunitionType GetAmmoType() const;

	bool CanShoot() const;

	FOnAmmoChanged OnAmmoChanged;
	
	FOnReloadComplete OnReloadComplete;

	virtual EReticleType GetReticleType() const;

	void ChangeAmmoType();

	void SetCurrentSlotsType(EAmunitionType Type);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon | Parameters | Ammo", meta = (ClampMin = 0, UIMin = 0))
		bool AvaliableOptionalAmmo = false;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		class USkeletalMeshComponent* WeaponMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		class UWeaponBarellComponent* WeaponBarell;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animations | Weapon")
		UAnimMontage* WeaponFireMontage;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animations | Weapon")
		UAnimMontage* WeaponReloadMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animations | Character")
		UAnimMontage* CharacterFireMontage;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animations | Character")
		UAnimMontage* CharacterReloadMontage;
	
	//FullClip reload type adds ammo only when the whole reload animation is successfully played
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animations | Character")
		EReloadType ReloadType = EReloadType::FullClip;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animations | Character")
		EReloadType OptionalReloadType = EReloadType::ByBullet;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon | Parameters")
		EWeaponFireMode WeaponFireMode = EWeaponFireMode::Single;

	//кол-во выстрелов в минуту
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon | Parameters", meta = (ClampMin = 1.0f, UIMin = 1.0f))
		float RateOfFire = 600.0f;

	//половина угла разброса
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon | Parameters", meta = (ClampMin = 0.0f, UIMin = 0.0f, ClampMax = 2.0f, UIMax = 2.0f))
		float SpreadAngle = 1.0f;
	//половина угла разброса во вермя прицеливания
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon | Parameters", meta = (ClampMin = 0.0f, UIMin = 0.0f, ClampMax = 2.0f, UIMax = 2.0f))
		float AimSpreadAngle = 0.25f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon | Parameters", meta = (ClampMin = 0.0f, UIMin = 0.0f, ClampMax = 2.0f, UIMax = 2.0f))
		float AimMovementMaxSpeed = 200.0f;

	//угл обзора камеры
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon | Parameters", meta = (ClampMin = 0.0f, UIMin = 0.0f, ClampMax = 120.0f, UIMax = 120.0f))
		float AimTurnModifier = 60.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon | Parameters", meta = (ClampMin = 0.0f, UIMin = 0.0f, ClampMax = 120.0f, UIMax = 120.0f))
		float AimingFOV = 60.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Aim | Modifiers", meta = (ClampMin = 0.0f, UIMin = 0.0f, ClampMax = 1.0f, UIMax = 1.0f))
		float AimingTurnModifier = 0.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Aim | Modifiers", meta = (ClampMin = 0.0f, UIMin = 0.0f, ClampMax = 1.0f, UIMax = 1.0f))
		float AimimgLookUpModifier = 0.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon | Parameters | Ammo", meta = (ClampMin = 1, UIMin = 1))
		int32 MaxAmmo = 30;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon | Parameters | Ammo", meta = (ClampMin = 0, UIMin = 0))
		int32 MaxOptionalAmmo = 0;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon | Parameters | Ammo")
		bool bAutoReload = true;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon | Parameters | Ammo")
		EAmunitionType AmmoType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Reticle")
		EReticleType AimReticleType = EReticleType::Default;

private:
	int32 Ammo;
	int32 OptionalAmmo;

	EAmunitionType CurrentSlotsType = EAmunitionType::None;

	bool bIsFiring = false;
	bool bIsReloading = false;
	bool IsChangedAmmoType = false;

	float GetCurrentBulletSpreadAngle() const;

	float CurrentAimingMovementSpeed;

	bool bIsAiming;

	void MakeShot();

	void OnShotTimerElapsed();

	//переводит RateOfFire в интервалы между выстрелами
	float GetShotTimerInterval() const;

	float PlayAnimMontage(UAnimMontage* AnimMontage);
	void StopAnimMontage(UAnimMontage* AnimMontage, float BlendOutTime = 0.0f);

	FTimerHandle ShotTimer;
	FTimerHandle ReloadTimer;
};
