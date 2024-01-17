// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GCBaseCharacterAnimInstance.h"
#include "UObject/WeakObjectPtrTemplates.h"
#include "GCBaseCharacterAnimInstance.h"
#include "FPPlayerAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class HOMEWORKPROJECT_API UFPPlayerAnimInstance : public UGCBaseCharacterAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeBeginPlay() override;

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character | Animations | FirstPerson")
		float PlayerCameraPitchAngle = 0.0f;

	TWeakObjectPtr<class AFPPlayerCharacter> CachedFirstPersonCharacterOwner;

private:
	float CalculateCameraPitchAngle() const;
};
