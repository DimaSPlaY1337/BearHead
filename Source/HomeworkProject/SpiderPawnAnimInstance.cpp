// Fill out your copyright notice in the Description page of Project Settings.


#include "SpiderPawnAnimInstance.h"
#include "SpiderPawn.h"

void USpiderPawnAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();
	checkf(TryGetPawnOwner()->IsA<ASpiderPawn>(), TEXT("use only SpiderPawnAnimInstance"));
	CachedSpiderOwner = StaticCast<ASpiderPawn*>(TryGetPawnOwner());
}

void USpiderPawnAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (!CachedSpiderOwner.IsValid())
	{
		return;
	}
	RightFrontFootEffectorLocation = FVector(CachedSpiderOwner->GetIKRightFrontFootOffset(), 0.0f, 0.0f);
	RightRearFootEffectorLocation = FVector(CachedSpiderOwner->GetIKRightRearFootOffset(), 0.0f, 0.0f);
	LeftFrontFootEffectorLocation = FVector(CachedSpiderOwner->GetIKLeftFrontFootOffset(), 0.0f, 0.0f);
	LeftRearFootEffectorLocation = FVector(CachedSpiderOwner->GetIKLeftRearFootOffset(), 0.0f, 0.0f);
}
