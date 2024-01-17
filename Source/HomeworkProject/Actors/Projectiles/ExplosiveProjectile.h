// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Projectiles/GCProjectile.h"
#include "ExplosionComponent.h"
#include "ExplosiveProjectile.generated.h"

/**
 * 
 */
UCLASS()
class HOMEWORKPROJECT_API AExplosiveProjectile : public AGCProjectile
{
	GENERATED_BODY()

public:
	AExplosiveProjectile();

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		UExplosionComponent* ExplosionComponent;
	//задержка между броском и взрывом гранаты
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Explosion")
		float DetonationTime = 2.0f;

	virtual void OnProjectileLaunched() override;

	virtual void Explode() override;

private:
	AController* GetController();

	FTimerHandle DetonationTimer;
};
