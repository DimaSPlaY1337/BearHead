// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Equipment/Weapons/RangeWeaponItem.h"
#include "GameCodeTypes.h"
#include "Animation/AnimInstance.h"
#include "GCBaseCharacter.h"

ARangeWeaponItem::ARangeWeaponItem()
{
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("WeaponRoot"));

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	WeaponMesh->SetupAttachment(RootComponent);

	WeaponBarell = CreateDefaultSubobject<UWeaponBarellComponent>(TEXT("WeaponBarell"));
	WeaponBarell->SetupAttachment(WeaponMesh, SocketWeaponMuzzle);

	ReticleType = EReticleType::Default;
	//базовое значение сокета
	EquippedSocketName = SocketCharacterWeapon;
}

void ARangeWeaponItem::StartFire()
{
	if (GetWorld()->GetTimerManager().IsTimerActive(ShotTimer))
	{
		return;
	}

	bIsFiring = true;
	MakeShot();
}

void ARangeWeaponItem::StopFire()
{
	bIsFiring = false;
}

void ARangeWeaponItem::StartAim()
{
	bIsAiming = true;
}

void ARangeWeaponItem::StopAim()
{
	bIsAiming = false;
}

bool ARangeWeaponItem::IsFiring() const
{
	return bIsFiring;
}

bool ARangeWeaponItem::IsReloading() const
{
	return bIsReloading;
}

void ARangeWeaponItem::StartReload()
{
	AGCBaseCharacter* CharacterOwner = GetCharacterOwner();
	if (!IsValid(CharacterOwner))
	{
		return;
	}

	bIsReloading = true;
	if (IsValid(CharacterReloadMontage))
	{
		float MontageDuration = CharacterOwner->PlayAnimMontage(CharacterReloadMontage);
		PlayAnimMontage(WeaponReloadMontage);
		if (ReloadType == EReloadType::FullClip)
		{
			GetWorld()->GetTimerManager().SetTimer(ReloadTimer, [this]() { EndReload(true); }, MontageDuration, false);
		}
	}
	else
	{
		EndReload(true);
	}
}

void ARangeWeaponItem::EndReload(bool bIsSuccess)
{
	if (!bIsReloading)
	{
		return;
	}

	AGCBaseCharacter* CharacterOwner = GetCharacterOwner();

	if (!bIsSuccess)
	{
		if (!IsValid(CharacterOwner))
		{
			CharacterOwner->StopAnimMontage(CharacterReloadMontage);
		}
		StopAnimMontage(WeaponReloadMontage);
	}

	if (ReloadType == EReloadType::ByBullet)
	{
		UAnimInstance* CharacterAnimInstance = IsValid(CharacterOwner) ? CharacterOwner->GetMesh()->GetAnimInstance() : nullptr;
		if (IsValid(CharacterAnimInstance))
		{
			CharacterAnimInstance->Montage_JumpToSection(SectionMontageReloadEnd, CharacterReloadMontage);
		}

		UAnimInstance* WeaponAnimInstance = WeaponMesh->GetAnimInstance();
		if (IsValid(WeaponAnimInstance))
		{
			WeaponAnimInstance->Montage_JumpToSection(SectionMontageReloadEnd, WeaponReloadMontage);
		}
	}

	GetWorld()->GetTimerManager().ClearTimer(ReloadTimer);

	bIsReloading = false;
	if (OnReloadComplete.IsBound() && bIsSuccess)
	{
		OnReloadComplete.Broadcast();
	}
}

float ARangeWeaponItem::GetAimFOV() const
{
	return AimingFOV;
}

float ARangeWeaponItem::GetAimMovementMaxSpeed() const
{
	return AimMovementMaxSpeed;
}

FTransform ARangeWeaponItem::GetForeGripTransform() const
{
	return WeaponMesh->GetSocketTransform(SocketWeaponForeGrip);
}

int32 ARangeWeaponItem::GetAmmo() const
{
	return Ammo;
}

int32 ARangeWeaponItem::GetMaxAmmo() const
{
	return MaxAmmo;
}

int32 ARangeWeaponItem::GetOptionalAmmo() const
{
	return OptionalAmmo;
}

int32 ARangeWeaponItem::GetMaxOptionalAmmo() const
{
	return MaxOptionalAmmo;
}

void ARangeWeaponItem::SetAmmo(int32 NewAmmo)
{
	Ammo = NewAmmo;
	if (OnAmmoChanged.IsBound())
	{
		OnAmmoChanged.Broadcast(Ammo);
	}
}

void ARangeWeaponItem::SetOptionalAmmo(int32 NewAmmo)
{
	OptionalAmmo = NewAmmo;
	if (OnAmmoChanged.IsBound())
	{
		OnAmmoChanged.Broadcast(OptionalAmmo);
	}
}

EAmunitionType ARangeWeaponItem::GetAmmoType() const
{
	if (CurrentSlotsType == EAmunitionType::None)
	{
		return AmmoType;
	}
	else
	{
		return CurrentSlotsType;
	}
}

bool ARangeWeaponItem::CanShoot() const
{
	if (IsChangedAmmoType)
	{
		return OptionalAmmo > 0;
	}
	else
	{
		return Ammo > 0;

	}
}

EReticleType ARangeWeaponItem::GetReticleType() const
{
	return bIsAiming ? AimReticleType : ReticleType;
}

void ARangeWeaponItem::ChangeAmmoType()
{
	WeaponBarell->ChangeRegistrationType();
	if (IsChangedAmmoType)
	{
		IsChangedAmmoType = false;
	}
	else
	{
		IsChangedAmmoType = true;
	}
}

void ARangeWeaponItem::BeginPlay()
{
	Super::BeginPlay();
	SetAmmo(MaxAmmo);
	SetOptionalAmmo(MaxOptionalAmmo);
}

float ARangeWeaponItem::GetCurrentBulletSpreadAngle() const
{
	float AngleInGegress = bIsAiming ? AimSpreadAngle : SpreadAngle;
	return FMath::DegreesToRadians(AngleInGegress);
}

void ARangeWeaponItem::SetCurrentSlotsType(EAmunitionType Type)
{
	CurrentSlotsType = Type;
}

void ARangeWeaponItem::MakeShot()
{
	AGCBaseCharacter* CharacterOwner = GetCharacterOwner();
	if (!CanShoot())
	{
		StopFire();
		if (Ammo == 0 && bAutoReload)
		{
			CharacterOwner->Reload();
		}
		return;
	}

	EndReload(false);
	CharacterOwner->PlayAnimMontage(CharacterFireMontage);
	PlayAnimMontage(WeaponFireMontage);

	FVector ShotLocation;
	FRotator ShotRotation;

	if (CharacterOwner->IsPlayerControlled())
	{
		APlayerController* Controller = CharacterOwner->GetController<APlayerController>();
		Controller->GetPlayerViewPoint(ShotLocation, ShotRotation);
	}
	else
	{
		ShotLocation = WeaponBarell->GetComponentLocation();
		ShotRotation = CharacterOwner->GetBaseAimRotation();
	}

	FVector ShotDirection = ShotRotation.RotateVector(FVector::ForwardVector);
	if (IsChangedAmmoType)
	{
		SetOptionalAmmo(OptionalAmmo - 1);
	}
	else
	{
		SetAmmo(Ammo - 1);
	}
	WeaponBarell->Shot(ShotLocation, ShotDirection, GetCurrentBulletSpreadAngle());
	GetWorld()->GetTimerManager().SetTimer(ShotTimer, this, &ARangeWeaponItem::OnShotTimerElapsed, GetShotTimerInterval(), false);
}

void ARangeWeaponItem::OnShotTimerElapsed()
{
	if (!bIsFiring)
	{
		return;
	}

	switch (WeaponFireMode)
	{
		case EWeaponFireMode::Single:
		{
			StopFire();
			break;
		}
		case EWeaponFireMode::FullAuto:
		{
			MakeShot();
			break;
		}
	}
}

float ARangeWeaponItem::GetShotTimerInterval() const
{
	return 60.0f / RateOfFire;
}

float ARangeWeaponItem::PlayAnimMontage(UAnimMontage* AnimMontage)
{
	UAnimInstance* WeaponAnimInstance = WeaponMesh->GetAnimInstance();
	float Result = 0.0f;
	if (IsValid(WeaponAnimInstance))
	{
		Result = WeaponAnimInstance->Montage_Play(AnimMontage);
	}
	return Result;
}

void ARangeWeaponItem::StopAnimMontage(UAnimMontage* AnimMontage, float BlendOutTime)
{
	UAnimInstance* WeaponAnimInstance = WeaponMesh->GetAnimInstance();
	if (IsValid(WeaponAnimInstance))
	{
		WeaponAnimInstance->Montage_Stop(BlendOutTime, AnimMontage);
	}
}
