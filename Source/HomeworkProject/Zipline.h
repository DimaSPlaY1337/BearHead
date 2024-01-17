// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractiveActor.h"
#include "Components/CapsuleComponent.h"
#include "Ladder.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "GCBaseCharacterMovementComponent.h"
#include "Zipline.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class HOMEWORKPROJECT_API AZipline : public AInteractiveActor
{
	GENERATED_BODY()
	
public:

	AZipline();

	virtual void OnConstruction(const FTransform& Transform) override;

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	FVector GetFirstLocation() const { return Point1; }

	FVector GetSecondLocation() const { return Point2; }

protected:

	class UCapsuleComponent* GetInterectionVolume() const;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ladder parametrs")
		float ZiplineHeight = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ladder parametrs")
		float CableLenght = 230.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UStaticMeshComponent* FirstPostComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UStaticMeshComponent* SecondPostComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UStaticMeshComponent* CableComponent;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, meta = (MakeEditWidget))
		FVector FirstPost = FVector(0.0f, 0.0f, ZiplineHeight);

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, meta = (MakeEditWidget))
		FVector SecondPost = FVector(0.0f, 0.0f, ZiplineHeight);

	virtual void OnInteractionVolumeOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	virtual void OnInteractionVolumeOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

private:
	FVector Point1;
	FVector Point2;
};
