// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "GameCodeTypes.h"
#include "GCBaseCharacterAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class HOMEWORKPROJECT_API UGCBaseCharacterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	virtual void NativeBeginPlay() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character animation")
	bool bIsFalling = false;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character animation")
	float Speed = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character animation")
	bool bIsCrouching = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character animation")
	bool bIsSprinting = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character animation")
	bool bIsOutOfStamina= false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character animation")
	bool bIsProning = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character animation")
	bool bIsSwimming = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character animation")
	bool bIsOnLadder = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character animation")
	bool bIsOnZipline = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character animation")
	bool bIsWallRunning = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character animation")
	bool bWallRunSide = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character animation")
	float LadderSpeedRatio = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character animation")
		bool bIsStrafing = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character animation", meta = (UIMin = -180.0f, UIMax = 180.0f))
		float Direction = 0.0f;

	UPROPERTY(EditAnywhere, Transient, BlueprintReadOnly, Category = "Character |IK Settings")
	FVector IKRightFootOffset = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, Transient, BlueprintReadOnly, Category = "Character |IK Settings")
	FVector IKLeftFootOffset = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, Transient, BlueprintReadOnly, Category = "Character |IK Settings")
	FVector IKPelvisBoneOffset = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character animation")
	EEquippedItemType CurrentEquippedItemType = EEquippedItemType::None;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character animation")
	FRotator AimRotation = FRotator::ZeroRotator;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character animation | Weapon")
	FTransform ForeGripSocketTransform;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character animation | Weapon")
		bool bIsAiming;

private:
	TWeakObjectPtr<class AGCBaseCharacter> CachedBaseCharacter;
};
