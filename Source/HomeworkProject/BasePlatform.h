// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GCBaseCharacter.h"
#include "GCBaseCharacterMovementComponent.h"
#include "Components/TimelineComponent.h"
#include "PlatformBehavior.h"
#include "LedgeDetectorComponent.h"
#include "PlatformInvocator.h"
#include "Components/BoxComponent.h"
#include "BasePlatform.generated.h"




UCLASS()
class HOMEWORKPROJECT_API ABasePlatform : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABasePlatform();

	/*FVector GetLocation() { return Component->GetRelativeLocation(); }*/

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void PlatformTimelineUpdate(float Alpha);

	void DoIfChoosePlatformBehavior();

	void DoIfChoosePlatformInvocator();

	UFUNCTION()
		void OnPlatformInvoked();

	FTimeline PlatformTimeline;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UBoxComponent* PrimitiveComponent;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, meta = (MakeEditWidget))
	FVector EndLocation;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Transient)
	FVector StartLocation;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int SleepTime;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* PlatformMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UCurveFloat* TimelineCurve;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EPlatformBehavior PlatformBehavior = EPlatformBehavior::OnDemand;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly)
	APlatformInvocator* PlatformInvocator;

	void PlatformLocation(FVector Location, FQuat Rotation, FCollisionShape Collision);

	void AddIngnorePlatfromCollision(FCollisionQueryParams QueryParams);

	//ULedgeDetectorComponent Component;
	bool IsNextToPlatform = 0;
	AGCBaseCharacter* Character;
	FVector BoxSize;
};