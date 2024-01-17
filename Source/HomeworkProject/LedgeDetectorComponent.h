// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/BoxComponent.h"
#include "LedgeDetectorComponent.generated.h"

USTRUCT(BlueprintType)
struct FledgeDescription
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ledge description")
	FVector Location;//точка на которую должен забраться персонаж

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ledge description")
	FVector LedgeNormal;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ledge description")
	FRotator Rotation;//чтобы персонаж смотрел нужную сторону
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class HOMEWORKPROJECT_API ULedgeDetectorComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	bool DetectLedge(OUT FledgeDescription& LedgeDescription);

	FVector GetDownwardTraceLocation() {
		return CachedDownwardTraceLocation;
	}

	FVector CharacterBottom;
	FVector DownwardStartLocation;
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	void SetDownwardTraceLocation(FVector DownwardTraceLocation) {
		CachedDownwardTraceLocation = DownwardTraceLocation;
	}

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Detection settings", meta = (UIMin = 0.0f, ClampMin = 0.0f))
		float MinimumLedgeHeight = 40.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Detection settings", meta = (UIMin = 0.0f, ClampMin = 0.0f))
		float MaximumLedgeHeight = 200.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Detection settings", meta = (UIMin = 0.0f, ClampMin = 0.0f))
		float ForwardCheckDistance = 100.0f;
private:
	TWeakObjectPtr<class ACharacter> CachedCharacterOwner;
	float OverlapCapsuleHalfHeight;
	FVector CachedDownwardTraceLocation;
	//ABasePlatform* platform;
};
