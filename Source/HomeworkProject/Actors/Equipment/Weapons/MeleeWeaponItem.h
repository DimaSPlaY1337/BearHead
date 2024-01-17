// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Equipment/EquipableItem.h"
#include "GameCodeTypes.h"
#include "Components/Weapon/MeleeHitRegistrator.h"
#include "MeleeWeaponItem.generated.h"

//хранит описание аттаки
USTRUCT(BlueprintType)
struct FMeleeAttackDescription
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Melee attack")
	TSubclassOf<class UDamageType> DamageTypeClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Melee attack", meta = (ClampMin = 0.0f, UIMin = 0.0f))
	float DamageAmount = 50.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Melee attack", meta = (ClampMin = 0.0f, UIMin = 0.0f))
	class UAnimMontage* AttackMontage;
};

class UMeleeHitRegistrator;
UCLASS(Blueprintable)
class HOMEWORKPROJECT_API AMeleeWeaponItem : public AEquipableItem
{
	GENERATED_BODY()

public:
	AMeleeWeaponItem();
	void StartAttack(EMeleeAttackTypes AttackType);
	void SetIsHitRegistrationEnabled(bool bIsRegistrationEnabled);

protected:
	//хранит доступные аттаки
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Melee attack")
	TMap<EMeleeAttackTypes, FMeleeAttackDescription> Attacks;

	virtual void BeginPlay() override;

private:
	UFUNCTION()
	void ProcessHit(const FHitResult& HitResult, const FVector& HitDirection);

	TArray<UMeleeHitRegistrator*> HitRegistrators;
	TSet<AActor*> HitActors;
	FMeleeAttackDescription* CurrentAttack;
	void OnAttackTimerElapsed();
	FTimerHandle AttackTimer;
};
