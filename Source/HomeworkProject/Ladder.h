// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractiveActor.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "Ladder.generated.h"

class UBoxComponent;
class UAnimMontage;
class UStaticMeshComponent;
/**
 * 
 */
UCLASS(Blueprintable)
class HOMEWORKPROJECT_API ALadder : public AInteractiveActor
{
	GENERATED_BODY()

public:
	ALadder();

	virtual void OnConstruction(const FTransform& Transform) override;

	virtual void BeginPlay() override;

	float GetLadderHeight() const;

	bool GetIsOnTop() const;

	UAnimMontage* GetAttachFromTopAnimMontage() const;

	FVector GetAttachFromTopAnimMontageStartingLocation() const;
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ladder parametrs")
		float LadderHeight = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ladder parametrs")
		float LadderWidth = 50.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ladder parametrs")
		float StepsInterval= 25.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ladder parametrs")
		float BottomStepOffset = 25.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* RightRailMeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* LeftRailMeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UInstancedStaticMeshComponent* StepsMeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UBoxComponent* TopInteractionVolume;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ladder parametrs")
	UAnimMontage* AttachFromTopAnimMontage;
	
	//Offset from ladder's top for starting anim montage
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ladder parametrs")
	FVector AttachFromTopAnimMontageInitialOffset = FVector::ZeroVector;

	class UBoxComponent* GetLadderInteractionBox() const;

	virtual void OnInteractionVolumeOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	virtual void OnInteractionVolumeOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

private:
	bool bIsOnTop = false;
};
