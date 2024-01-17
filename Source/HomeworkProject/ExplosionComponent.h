// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "ExplosionComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnExplosion);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class HOMEWORKPROJECT_API UExplosionComponent : public USceneComponent
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	void Explode(AController* Controller);

	UPROPERTY(BlueprintAssignable)
	FOnExplosion OnExplosion;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Explosion | Damage")
		float MaxDamage = 75.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Explosion | Damage")
		float MinDamage = 30.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Explosion | Damage")
		float DamageFallOff = 1.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Explosion | Damage")
		TSubclassOf<class UDamageType> DamageTypeClass;
	//радиус на растоянии от которого наноситься макс урон
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Explosion | Radius")
		float InnerRadius = 300.0f;
	//радиус на котором урон наноситься минимальный
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Explosion | Radius")
		float OtherRadius = 500.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Explosion | VFX")
		UParticleSystem* ExplosionVFX;
};
