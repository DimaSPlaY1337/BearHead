// Fill out your copyright notice in the Description page of Project Settings.


#include "SpiderPawn.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Engine/EngineTypes.h"

ASpiderPawn::ASpiderPawn()
{
	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Spider mesh"));
	SkeletalMeshComponent->SetupAttachment(RootComponent);

	IKScale = GetActorScale3D().Z;
	IKTraceDistance = CollisionSphereRadius * IKScale;
}

void ASpiderPawn::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	IKRightFrontFootOffset = FMath::FInterpTo(IKRightFrontFootOffset, GetIKOffsetForASocket(RightFrontFootSocketName), DeltaSeconds, IKInterpSpeed);
	IKRightRearFootOffset = FMath::FInterpTo(IKRightRearFootOffset, GetIKOffsetForASocket(RightRearFootSocketName), DeltaSeconds, IKInterpSpeed);
	IKLeftFrontFootOffset = FMath::FInterpTo(IKLeftFrontFootOffset, GetIKOffsetForASocket(LeftFrontFootSocketName), DeltaSeconds, IKInterpSpeed);
	IKLeftRearFootOffset = FMath::FInterpTo(IKLeftRearFootOffset, GetIKOffsetForASocket(LeftRearFootSocketName), DeltaSeconds, IKInterpSpeed);
}

float ASpiderPawn::GetIKOffsetForASocket(const FName& SocketName)
{
	float Result = 0.0f;

	FVector SocketLocation = SkeletalMeshComponent->GetSocketLocation(SocketName);
	FVector TraceStart(SocketLocation.X, SocketLocation.Y, GetActorLocation().Z);
	FVector TraceEnd = TraceStart - IKTraceDistance * FVector::UpVector;

	FHitResult HitResult;
	ETraceTypeQuery TraceType = UEngineTypes::ConvertToTraceType(ECC_Visibility);
	//result расстояние между позицией когда нога выровненна с полом и местом колизии
	if (UKismetSystemLibrary::LineTraceSingle(GetWorld(), TraceStart, TraceEnd, TraceType, true, TArray<AActor*>(), EDrawDebugTrace::ForOneFrame, HitResult, true))
	{ 
		Result = (TraceEnd.Z - HitResult.Location.Z) / IKScale;
	}
	else if (UKismetSystemLibrary::LineTraceSingle(GetWorld(), TraceEnd, TraceEnd - IKTraceExtendDistance * FVector::UpVector, TraceType, true, TArray<AActor*>(), EDrawDebugTrace::ForOneFrame, HitResult, true))
	{ 
		Result = (TraceEnd.Z - HitResult.Location.Z) / IKScale;
	}
		
	return Result;
}
