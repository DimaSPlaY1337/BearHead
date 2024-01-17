// Fill out your copyright notice in the Description page of Project Settings.


#include "FPPlayerAnimInstance.h"
#include "GCPlayerController.h"
#include "FPPlayerCharacter.h"

void UFPPlayerAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();
	checkf(GetOwningActor()->IsA<AFPPlayerCharacter>(), TEXT("UFPPlayerAnimInstance::NativeBeginPlay work only with AFPPlayerCharacter"));
	CachedFirstPersonCharacterOwner = StaticCast<AFPPlayerCharacter*>(TryGetPawnOwner());
}

void UFPPlayerAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	if (!CachedFirstPersonCharacterOwner.IsValid())
	{
		return;
	}
	PlayerCameraPitchAngle = CalculateCameraPitchAngle();
}

float UFPPlayerAnimInstance::CalculateCameraPitchAngle() const
{
	float Result = 0.0f;
	AGCPlayerController* Controller = CachedFirstPersonCharacterOwner->GetController<AGCPlayerController>();
	if (IsValid(Controller) && !Controller->GetIgnoreCameraPitch())
	{
		Result = Controller->GetControlRotation().Pitch;
	}
	return Result;
}
