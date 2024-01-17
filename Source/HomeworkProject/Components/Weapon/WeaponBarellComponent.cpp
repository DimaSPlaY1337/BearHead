// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Weapon/WeaponBarellComponent.h"
#include "GameCodeTypes.h"
#include "DrawDebugHelpers.h"
#include "Subsystems/DebugSubsystem.h"
#include "Kismet/GamePlayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "Actors/Projectiles/GCProjectile.h"
#include "NiagaraComponent.h"
#include "Components/DecalComponent.h"

void UWeaponBarellComponent::Shot(FVector ShotStart, FVector ShotDirection, float SpreadAngle)
{
	FVector MuzzleLocation = GetComponentLocation();
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), MuzzleFlashFX, MuzzleLocation, GetComponentRotation());
	for (int i = 0; i < BulletPerShot; i++)
	{
		ShotDirection += GetBulletSpreadOffset(FMath::RandRange(0.0f, SpreadAngle), ShotDirection.ToOrientationRotator());

		FVector ShotEnd = ShotStart + FiringRange * ShotDirection;

#if ENABLE_DRAW_DEBUG
		UDebugSubsystem* DebugSubSystem = UGameplayStatics::GetGameInstance(GetWorld())->GetSubsystem<UDebugSubsystem>();
		bool bIsDebugEnabled = DebugSubSystem->IsCategoryEnabled(DebugCategoryRangeWeapon);
#else
		bool bIsDebugEnabled = false;
#endif

		switch (HitRegistration)
		{
			case EHitRegistrationType::HitScan:
			{
				bool bHasHit = HitScan(ShotStart, ShotEnd, ShotDirection);
				if (bIsDebugEnabled && bHasHit)
				{
					DrawDebugSphere(GetWorld(), ShotEnd, 10.0f, 24, FColor::Red, false, 1.0f);
				}
				break;
			}
			case EHitRegistrationType::Projectile:
			{
				LaunchProjectile(ShotStart, ShotDirection);
				break;
			}
		}
		UNiagaraComponent* TraceFXComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), TraceFX, MuzzleLocation, GetComponentRotation());
		TraceFXComponent->SetVectorParameter(FXParamTraceEnd, ShotEnd);//задаём вектор полёта пули

		if (bIsDebugEnabled)
		{
			DrawDebugLine(GetWorld(), MuzzleLocation, ShotEnd, FColor::Red, false, 1.0f, 0, 3.0f);
		}
	}
}

bool UWeaponBarellComponent::HitScan(FVector ShotStart, OUT FVector& ShotEnd, FVector ShotDirection)
{
	FHitResult ShotResult;
	bool HasHit = GetWorld()->LineTraceSingleByChannel(ShotResult, ShotStart, ShotEnd, ECC_Bullet);
	if (GetWorld()->LineTraceSingleByChannel(ShotResult, ShotStart, ShotEnd, ECC_Bullet))
	{
		ShotEnd = ShotResult.ImpactPoint;
		ProcessHit(ShotResult, ShotDirection);
	}
	return HasHit;
}

void UWeaponBarellComponent::LaunchProjectile(const FVector& LaunchStart, const FVector& LaunchDirection)
{
	AGCProjectile* Projectile = GetWorld()->SpawnActor<AGCProjectile>(ProjectileClass, LaunchStart, LaunchDirection.ToOrientationRotator());
	if (IsValid(Projectile))
	{
		Projectile->SetOwner(GetOwningPawn());
		Projectile->OnProjectileHit.AddDynamic(this, &UWeaponBarellComponent::ProcessHit);
		Projectile->LaunchProjectile(LaunchDirection.GetSafeNormal());
	}
}

void UWeaponBarellComponent::ChangeRegistrationType()
{	
	switch (HitRegistration)
	{
		case EHitRegistrationType::HitScan:
		{
			HitRegistration = EHitRegistrationType::Projectile;
			break;
		}
		case EHitRegistrationType::Projectile:
		{
			HitRegistration = EHitRegistrationType::HitScan;
			break;
		}
	}
}

float UWeaponBarellComponent::CalculateValueOffFallDamage(FHitResult ShotResult)
{
	float Value = 1.0f;
	if (IsValid(FalloffDiagram))
	{
		FVector CharacterLocation = GetOwner()->GetActorLocation();
		FVector ActorLocation = ShotResult.GetActor()->GetActorLocation();
		float Distance = FVector::Dist(CharacterLocation, ActorLocation) - DamageOffset;
		Value = FalloffDiagram->GetFloatValue(Distance);
	}
	return Value;
}

void UWeaponBarellComponent::ProcessHit(const FHitResult& HitResult, const FVector& Direction)
{
	AActor* HitActor = HitResult.GetActor();
	if (IsValid(HitActor))
	{
		FPointDamageEvent DamageEvent;
		DamageEvent.HitInfo = HitResult;
		DamageEvent.ShotDirection = Direction;
		DamageEvent.DamageTypeClass = DamageTypeClass;
		HitActor->TakeDamage(DamageAmount * CalculateValueOffFallDamage(HitResult), DamageEvent, GetController(), GetOwner());
	}
	UDecalComponent* DecalComponent = UGameplayStatics::SpawnDecalAtLocation(GetWorld(), DefaultDecalInfo.DecalMaterial, DefaultDecalInfo.DecalSize, HitResult.ImpactPoint, HitResult.ImpactNormal.ToOrientationRotator());
	if (IsValid(DecalComponent))
	{
		DecalComponent->SetFadeScreenSize(0.0001f);
		DecalComponent->SetFadeOut(DefaultDecalInfo.DecalLifeTime, DefaultDecalInfo.DecalFadeOutTime);
	}
}

APawn* UWeaponBarellComponent::GetOwningPawn() const
{
	//владелец бэрэл компонента
	APawn* PawnOwner = Cast<APawn>(GetOwner());
	if (IsValid(PawnOwner))
	{
		PawnOwner = Cast<APawn>(GetOwner()->GetOwner());
	}
	return PawnOwner;
}

AController* UWeaponBarellComponent::GetController() const
{
	APawn* PawnOwner = GetOwningPawn();
	return IsValid(PawnOwner) ? PawnOwner->GetController() : nullptr;
}

FVector UWeaponBarellComponent::GetBulletSpreadOffset(float Angle, FRotator ShotRotation) const
{
	float SpreadSize = FMath::Tan(Angle);
	float RotationAngle = FMath::RandRange(0.0f, 2 * PI);

	float SpreadY = FMath::Cos(RotationAngle);
	float SpreadZ = FMath::Sin(RotationAngle);

	FVector Result = (ShotRotation.RotateVector(FVector::UpVector) * SpreadZ
		+ ShotRotation.RotateVector(FVector::RightVector) * SpreadY) * SpreadSize;

	return Result;
}
