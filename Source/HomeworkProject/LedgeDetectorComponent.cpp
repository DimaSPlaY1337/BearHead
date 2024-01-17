// Fill out your copyright notice in the Description page of Project Settings.


#include "LedgeDetectorComponent.h"

#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"
#include "DrawDebugHelpers.h"
#include "Utils/GCTraceUtils.h"
#include "Kismet/GameplayStatics.h"
#include "GCGameInstance.h"
#include "HomeworkProject/GameCodeTypes.h"
#include "Subsystems/DebugSubsystem.h"

// Called when the game starts
void ULedgeDetectorComponent::BeginPlay()
{
	Super::BeginPlay();
	checkf(GetOwner()->IsA<ACharacter>(), TEXT("only character can use ULedgeDetectorComponent"));
	CachedCharacterOwner = StaticCast<ACharacter*>(GetOwner());
	OverlapCapsuleHalfHeight = CachedCharacterOwner->GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight();
}

bool ULedgeDetectorComponent::DetectLedge(OUT FledgeDescription& LedgeDescription)
{
	UCapsuleComponent* CapsuleComponent = CachedCharacterOwner->GetCapsuleComponent();
	FCollisionQueryParams QueryParams;
	QueryParams.bTraceComplex = true;
	QueryParams.AddIgnoredActor(GetOwner());
	
#if ENABLE_DRAW_DEBUG
	UDebugSubsystem* DebugSubSystem = UGameplayStatics::GetGameInstance(GetWorld())->GetSubsystem<UDebugSubsystem>();
	bool bIsDebugEnabled = DebugSubSystem->IsCategoryEnabled(DebugCategoryLedgeDetection);
#else
	bool bIsDebugEnabled = false;
#endif
	float BottomZOffset = 2.0f;
	CharacterBottom = CachedCharacterOwner->GetActorLocation() - (CapsuleComponent->GetUnscaledCapsuleHalfHeight() - BottomZOffset) * FVector::UpVector;
	float DrawTime = 2.0f;
	UE_LOG(LogTemp, Error, TEXT("CharacterBottom: GetActorLocation - %f; Delta - %f"), CachedCharacterOwner->GetActorLocation().Z, CapsuleComponent->GetUnscaledCapsuleHalfHeight() - BottomZOffset);

	//1. Forward check
	float ForwardCheckCapsuleRadius = CapsuleComponent->GetScaledCapsuleRadius();
	float ForwardCheckCapsuleHalfHeight = (MaximumLedgeHeight - MinimumLedgeHeight) * 0.5f;

	FHitResult ForwardCheckHitResult;
	FCollisionShape ForwardCapsuleShape = FCollisionShape::MakeCapsule(ForwardCheckCapsuleRadius, ForwardCheckCapsuleHalfHeight);
	FVector ForwardStartLocation = CharacterBottom + (MinimumLedgeHeight + ForwardCheckCapsuleHalfHeight) * FVector::UpVector;
	FVector ForwardEndLocation = ForwardStartLocation + CachedCharacterOwner->GetActorForwardVector() * ForwardCheckDistance;

	if (!GCTraceUtils::SweepCapsuleSingleByChanel(GetWorld(),ForwardCheckHitResult,ForwardStartLocation,ForwardEndLocation, ForwardCheckCapsuleRadius, ForwardCheckCapsuleHalfHeight,FQuat::Identity, ECC_Climbing, QueryParams,FCollisionResponseParams::DefaultResponseParam, bIsDebugEnabled, DrawTime))
	{
		return false;
	}

	//2. Downward check
	FHitResult DownwardCheckHitResult;
	float DownwardSphereCheckRadius = CapsuleComponent->GetScaledCapsuleRadius();
	FCollisionShape DownwardSphereShape = FCollisionShape::MakeSphere(DownwardSphereCheckRadius);

	float DownwardCheckDepthOffset = 17.0f;
	DownwardStartLocation = ForwardCheckHitResult.ImpactPoint - ForwardCheckHitResult.ImpactNormal * DownwardCheckDepthOffset;
	DownwardStartLocation.Z = CharacterBottom.Z + MaximumLedgeHeight + DownwardSphereCheckRadius;
	FVector DownwardTraceLocation(DownwardStartLocation.X, DownwardStartLocation.Y, CharacterBottom.Z);
	SetDownwardTraceLocation(DownwardTraceLocation);

	if (!GCTraceUtils::SweepSphereSingleByChanel(GetWorld(),DownwardCheckHitResult,DownwardStartLocation,DownwardTraceLocation,DownwardSphereCheckRadius,ECC_Climbing,QueryParams,FCollisionResponseParams::DefaultResponseParam, bIsDebugEnabled,DrawTime))
	{
		return false;
	}

	//3. Overlap check проверка что нет препятствий
	float OverlapCapsuleRadius = CapsuleComponent->GetScaledCapsuleRadius();
	FCollisionShape OverlapCapsuleShape = FCollisionShape::MakeCapsule(OverlapCapsuleRadius, OverlapCapsuleHalfHeight);
	float OverlapCapsuleFloorOffset = 2.0f;
	FVector OverlapLocation = DownwardCheckHitResult.ImpactPoint + (OverlapCapsuleHalfHeight + OverlapCapsuleFloorOffset) * FVector::UpVector;
	if (GCTraceUtils::OverlapCapsuleBlockingByProfile(GetWorld(), OverlapLocation, OverlapCapsuleRadius, OverlapCapsuleHalfHeight, FQuat::Identity, CollisionProfilePawn, QueryParams, bIsDebugEnabled, DrawTime))
	{
		return false;
	}

	LedgeDescription.Location = OverlapLocation;
	LedgeDescription.Rotation = (ForwardCheckHitResult.ImpactNormal * FVector(-1.0f, -1.0f, 0.0f)).ToOrientationRotator();
	LedgeDescription.LedgeNormal = ForwardCheckHitResult.ImpactNormal;

	return true;
}
