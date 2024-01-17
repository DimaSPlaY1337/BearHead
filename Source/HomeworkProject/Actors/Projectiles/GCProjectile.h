// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GCProjectile.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnProjectileHit, const FHitResult&, Hit, const FVector&, Direction);
//DECLARE_DYNAMIC_MULTICAST_DELEGATE(FHitBeforeExplode);


UCLASS()
class HOMEWORKPROJECT_API AGCProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	AGCProjectile();

	UFUNCTION(BlueprintCallable)
	void LaunchProjectile(FVector Direction);

	UPROPERTY(BlueprintAssignable)
		FOnProjectileHit OnProjectileHit;

protected:
	//FHitBeforeExplode HitBeforeExplode;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		class USphereComponent* CollisionComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		class UProjectileMovementComponent* ProjectileMovementComponent;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Behavoiur")
		bool ExplodeWhenHit = false;

	virtual void BeginPlay() override;

	virtual void OnProjectileLaunched();

	virtual void Explode();

private:
	UFUNCTION()
		void OnCollisionHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
};
