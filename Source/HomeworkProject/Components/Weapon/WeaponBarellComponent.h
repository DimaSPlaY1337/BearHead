// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "WeaponBarellComponent.generated.h"

UENUM(BlueprintType)
enum class EHitRegistrationType : uint8
{
	HitScan,
	Projectile
};

USTRUCT(BlueprintType)
struct FDecalInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Decal info")
		UMaterialInterface* DecalMaterial;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Decal info")
		FVector DecalSize = FVector(5.0f, 5.0f, 5.0f);

	//врем€ на экране
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Decal info")
		float DecalLifeTime = 10.0f;

	//врем€ за которое деталь исчезает
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Decal info")
		float DecalFadeOutTime = 5.0f;
};

class UNiagaraSystem;
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class HOMEWORKPROJECT_API UWeaponBarellComponent : public USceneComponent
{
	GENERATED_BODY()

public:
	void Shot(FVector ShotStart, FVector ShotDirection, float SpreadAngle);

	void ChangeRegistrationType();
protected:
	float CalculateValueOffFallDamage(FHitResult ShotResult);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Barell attributes")
		float FiringRange = 5000.0f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Barell attributes", meta = (ClampMin = 1, UIMin = 1))
		int32 BulletPerShot = 1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Barell attributes | Hit registration")
		EHitRegistrationType HitRegistration = EHitRegistrationType::HitScan;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Barell attributes | Hit registration", meta = (EditCondition = "HitRegistrationType::Projectile"))
		TSubclassOf<class AGCProjectile> ProjectileClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Barell attributes | Damage")
		float DamageAmount = 20.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Barell attributes | Damage")
		UCurveFloat* FalloffDiagram;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Barell attributes | Damage")
		float DamageOffset = 100.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Barell attributes | Damage")
		TSubclassOf<class UDamageType> DamageTypeClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Barell attributes | VFX")
		UNiagaraSystem* MuzzleFlashFX;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Barell attributes | VFX")
		UNiagaraSystem* TraceFX;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Barell attributes | Decals")
		FDecalInfo DefaultDecalInfo;

private:

	bool HitScan(FVector ShotStart, OUT FVector& ShotEnd, FVector ShotDirection);
	void LaunchProjectile(const FVector& LaunchStart, const FVector& LaunchDirection);

	UFUNCTION()
		void ProcessHit(const FHitResult& HitResult, const FVector& Direction);

	APawn* GetOwningPawn() const;
	AController* GetController() const;

	FVector GetBulletSpreadOffset(float Angle, FRotator ShotRotation) const;
};
