// Fill out your copyright notice in the Description page of Project Settings.


#include "GCBaseCharacter.h"
#include "Zipline.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GCBaseCharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "LedgeDetectorComponent.h"
#include "Actors/Equipment/Weapons/MeleeWeaponItem.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SkeletalMeshComponent.h"
#include "CharacterAttributesComponent.h"
#include "Runtime/Engine/Classes/Kismet/KismetSystemLibrary.h"
#include "GCBaseCharacterAnimInstance.h"
#include "GameCodeTypes.h"
#include "Actors/Equipment/Weapons/RangeWeaponItem.h"
#include "Components/CharacterComponents/CharacterEquipmentComponent.h"
#include "AI/Controllers/AITurretController.h"

AGCBaseCharacter::AGCBaseCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UGCBaseCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	GCBaseCharacterMovementComponent = StaticCast<UGCBaseCharacterMovementComponent*>(GetCharacterMovement());

	IKScale = GetActorScale3D().Z;
	IKTraceDistance = 200 * IKScale;

	LedgeDetectorComponent = CreateDefaultSubobject<ULedgeDetectorComponent>(TEXT("LedgeDetector"));

	GetMesh()->CastShadow = true;
	GetMesh()->bCastDynamicShadow = true;

	CharacterAttributesComponent = CreateDefaultSubobject<UCharacterAttributesComponent>(TEXT("CharacterAttributes"));
	CharacterEquipmentComponent = CreateDefaultSubobject<UCharacterEquipmentComponent>(TEXT("CharacterEquipment"));
}

void AGCBaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	GetCapsuleComponent()->OnComponentHit.AddDynamic(GCBaseCharacterMovementComponent, &UGCBaseCharacterMovementComponent::OnPlayerCapsuleHit);

	CharacterAttributesComponent->OnDeathEvent.AddUObject(this, &AGCBaseCharacter::OnDeath);
	CharacterAttributesComponent->OutOfStamina.AddUObject(GetBaseCharacterMovementComponent(), &UGCBaseCharacterMovementComponent::SetIsOutOfStamina);
}

void AGCBaseCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	AAIController* AIController = Cast<AAIController>(NewController);
	if (IsValid(AIController))
	{
		FGenericTeamId TeamId((uint8)Team);
		AIController->SetGenericTeamId(TeamId);
	}
}

void AGCBaseCharacter::ChangeCrouchState()
{
	if (GetCharacterMovement()->IsCrouching() && !GCBaseCharacterMovementComponent->IsProning())
	{
		UnCrouch();
	}
	else if (!bIsSprintRequested && !GCBaseCharacterMovementComponent->IsSwimming() && !GCBaseCharacterMovementComponent->IsSliding())
	{
		Crouch();
		if (GCBaseCharacterMovementComponent->IsProning())
		{
			ChangeProneState();
		}
	}
}

void AGCBaseCharacter::ChangeProneState()
{
	if (GCBaseCharacterMovementComponent->IsCrouching() && !GCBaseCharacterMovementComponent->IsProning())
	{
		Prone();
	}
	else if (GCBaseCharacterMovementComponent->IsProning() && !GCBaseCharacterMovementComponent->IsEncroached())
	{
		UnProne();
	}
}

void AGCBaseCharacter::TryToJump()
{
	if (!GCBaseCharacterMovementComponent->IsEncroached() && !GCBaseCharacterMovementComponent->IsProning() && !GCBaseCharacterMovementComponent->IsSwimming() && !GCBaseCharacterMovementComponent->IsCrouching())
	{
		Jump();
	}
	else if (!GCBaseCharacterMovementComponent->IsProning() && GCBaseCharacterMovementComponent->IsCrouching() && !GCBaseCharacterMovementComponent->IsEncroached() && !GCBaseCharacterMovementComponent->IsSwimming())
	{
		ChangeCrouchState();
	}
	else if ((GCBaseCharacterMovementComponent->IsProning() || GCBaseCharacterMovementComponent->IsCrouching()) && !GCBaseCharacterMovementComponent->IsEncroached() && !GCBaseCharacterMovementComponent->IsSwimming())
	{
		ChangeProneState();
		ChangeCrouchState();
	}
}

void AGCBaseCharacter::StartSprint()
{
	bIsSprintRequested = true;
	if (bIsCrouched && !GCBaseCharacterMovementComponent->IsProning())
	{
		UnCrouch();
	}
}

void AGCBaseCharacter::StopSprint()
{
	bIsSprintRequested = false;
}

void AGCBaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	TryChangeSprintState(DeltaTime);
	IKLeftFootOffset = FMath::FInterpTo(IKLeftFootOffset, GetIKOffsetForASocket(LeftFootSocketName), DeltaTime, IKInterpSpeed);
	IKRightFootOffset = FMath::FInterpTo(IKRightFootOffset, GetIKOffsetForASocket(RightFootSocketName), DeltaTime, IKInterpSpeed);
	IKPelvisOffset = FMath::FInterpTo(IKPelvisOffset, CalculatePelvisOffset(), DeltaTime, IKInterpSpeed);
	if (HealthPercentEvent.IsBound())
	{
		HealthPercentEvent.Broadcast();
	}
}

bool AGCBaseCharacter::CanJumpInternal_Implementation() const
{
	return Super::CanJumpInternal_Implementation() && !GetBaseCharacterMovementComponent()->IsMantling();
}

void AGCBaseCharacter::Mantle(bool bForce /*= false*/)
{
	if (!(CanMantle() || bForce))
	{
		return;
	}

	if (GCBaseCharacterMovementComponent->IsProning())
	{
		ChangeProneState();
	}
	if (GCBaseCharacterMovementComponent->IsCrouching())
	{
		ChangeCrouchState();
	}
	FledgeDescription LedgeDescription;
	if (LedgeDetectorComponent->DetectLedge(LedgeDescription) && !GCBaseCharacterMovementComponent->IsMantling())
	{
		FMantlingMovementParameters MantlingParamters;
		MantlingParamters.InitialRotation = GetActorRotation();
		MantlingParamters.TargetRotation = LedgeDescription.Rotation;
		MantlingParamters.TargetLocation = LedgeDescription.Location;
		MantlingParamters.InitialLocation = GetActorLocation();
		float MantlingHeight = (MantlingParamters.TargetLocation - MantlingParamters.InitialLocation).Z;
		float LedgeHeight = MantlingHeight - LedgeDetectorComponent->CharacterBottom.Z;
		const FMantlingSettings& MantlingSettings = GetMantlingSettings(LedgeHeight);

		float MinRange;
		float MaxRange;
		MantlingSettings.MantlingCurve->GetTimeRange(MinRange, MaxRange);

		MantlingParamters.Duration = MaxRange - MinRange;

		MantlingParamters.MantlingCurve = MantlingSettings.MantlingCurve;
		//интервалы высоты и времени, высота препятствия
		FVector2D SourceRange(MantlingSettings.MinHeight, MantlingSettings.MaxHeight);
		FVector2D TargetRange(MantlingSettings.MinHeightStartTime, MantlingSettings.MaxHeightStartTime);
		MantlingParamters.StartTime = FMath::GetMappedRangeValueClamped(SourceRange, TargetRange, MantlingHeight);

		MantlingParamters.InitialAnimationLocation = MantlingParamters.TargetLocation - MantlingSettings.AnimationCorrectionZ * FVector::UpVector + MantlingSettings.AnimationCorrectionXY * LedgeDescription.LedgeNormal;

		GCBaseCharacterMovementComponent->StartMantle(MantlingParamters);

		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		AnimInstance->Montage_Play(MantlingSettings.MantlingMontage, 1.0f, EMontagePlayReturnType::Duration, MantlingParamters.StartTime);
		OnMantle(MantlingSettings, MantlingParamters.StartTime);
	}
}

void AGCBaseCharacter::RegisterInteractiveActor(AInteractiveActor* InteractiveActor)
{
	AvailableInteractiveActors.AddUnique(InteractiveActor);
}

void AGCBaseCharacter::UnregisterInteractiveActor(AInteractiveActor* InteractiveActor)
{
	AvailableInteractiveActors.RemoveSingleSwap(InteractiveActor);
}

void AGCBaseCharacter::Falling()
{
	GetBaseCharacterMovementComponent()->bNotifyApex = true;
}

void AGCBaseCharacter::NotifyJumpApex()
{
	Super::NotifyJumpApex();
	CurrentFallApex = GetActorLocation();
}

void AGCBaseCharacter::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);
	float FallHeight = (CurrentFallApex - GetActorLocation()).Z * 0.01f;
	if (IsValid(FallDamageCurve))
	{
		float DamageAmount = FallDamageCurve->GetFloatValue(FallHeight);
		TakeDamage(DamageAmount, FDamageEvent(), GetController(), Hit.Actor.Get());
	}
}

void AGCBaseCharacter::PrimaryMeleeAttack()
{
	AMeleeWeaponItem* CurrentMeleeWeapon = CharacterEquipmentComponent->GetCurrentMeleeWeapon();
	if (IsValid(CurrentMeleeWeapon))
	{
		CurrentMeleeWeapon->StartAttack(EMeleeAttackTypes::PrimaryAttack);
	}
}

void AGCBaseCharacter::SecondaryMeleeAttack()
{
	AMeleeWeaponItem* CurrentMeleeWeapon = CharacterEquipmentComponent->GetCurrentMeleeWeapon();
	if (IsValid(CurrentMeleeWeapon))
	{
		CurrentMeleeWeapon->StartAttack(EMeleeAttackTypes::SecondaryAttack);
	}
}

FGenericTeamId AGCBaseCharacter::GetGenericTeamId() const
{
	return FGenericTeamId((uint8)Team);
}

void AGCBaseCharacter::ClimbLadderUp(float Value)
{
	if (GetBaseCharacterMovementComponent()->IsOnLadder() && !FMath::IsNearlyZero(Value))
	{
		FVector LadderUpVector = GetBaseCharacterMovementComponent()->GetCurrentLadder()->GetActorUpVector();
		AddMovementInput(LadderUpVector, Value);
	}
}

void AGCBaseCharacter::InteractWithLadder()
{
	if (GetBaseCharacterMovementComponent()->IsOnLadder())
	{
		GetBaseCharacterMovementComponent()->DetachFromLadder(EDetachFromLadderMethod::JumpOff);
	}
	else
	{
		const ALadder* AvailableLadder = GetAvailableLadder();
		if (IsValid(AvailableLadder))
		{
			if (AvailableLadder->GetIsOnTop())
			{
				PlayAnimMontage(AvailableLadder->GetAttachFromTopAnimMontage());
			}
			GetBaseCharacterMovementComponent()->AttachToLadder(AvailableLadder);
		}
	}
}

void AGCBaseCharacter::InteractWithZipline()
{
	if (GetBaseCharacterMovementComponent()->IsOnZipline())
	{
		GetBaseCharacterMovementComponent()->DetachFromZipline();
	}
	else
	{
		const AZipline* AvailableZipline = GetAvailableZipline();
		if (IsValid(AvailableZipline))
		{
			GetBaseCharacterMovementComponent()->AttachToZipline(AvailableZipline);
		}
	}
}

void AGCBaseCharacter::Sliding()
{
	if (GCBaseCharacterMovementComponent->IsSprinting() && !GCBaseCharacterMovementComponent->IsSliding())
	{
		GCBaseCharacterMovementComponent->StartSlide();
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		AnimInstance->Montage_Play(GetBaseCharacterMovementComponent()->SlideMontage, GetBaseCharacterMovementComponent()->SlideMaxTime, EMontagePlayReturnType::Duration, 0.0f);
	}
}

const ALadder* AGCBaseCharacter::GetAvailableLadder() const
{
	const ALadder* Result = nullptr;
	for (const AInteractiveActor* InteractiveActor : AvailableInteractiveActors)
	{
		if (InteractiveActor->IsA<ALadder>())
		{
			Result = StaticCast<const ALadder*>(InteractiveActor);
			break;
		}
	}
	return Result;
}

const AZipline* AGCBaseCharacter::GetAvailableZipline() const
{
	const AZipline* Result = nullptr;
	for (const AInteractiveActor* InteractiveActor : AvailableInteractiveActors)
	{
		if (InteractiveActor->IsA<AZipline>())
		{
			Result = StaticCast<const AZipline*>(InteractiveActor);
			break;
		}
	}
	return Result;
}

void AGCBaseCharacter::OnSprintStart_Implementation()
{
	UE_LOG(LogTemp, Log, TEXT("AGCBaseCharacter::OnSprintStart_Implementation()"));
}

void AGCBaseCharacter::OnSprintEnd_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("AGCBaseCharacter::OnSprintStart_Implementation()"));
}

void AGCBaseCharacter::StartFire()
{
	if (CharacterEquipmentComponent->IsEquipping())
	{
		return;
	}
	ARangeWeaponItem* CurrentRangeWeapon = CharacterEquipmentComponent->GetCurrentRangeWeapon();
	if (IsValid(CurrentRangeWeapon))
	{
		CurrentRangeWeapon->StartFire();
	}
}

void AGCBaseCharacter::StopFire()
{
	ARangeWeaponItem* CurrentRangeWeapon = CharacterEquipmentComponent->GetCurrentRangeWeapon();
	if (IsValid(CurrentRangeWeapon))
	{
		CurrentRangeWeapon->StopFire();
	}
}

void AGCBaseCharacter::StartAiming()
{
	ARangeWeaponItem* CurrentRangeWeapon = GetCharacterEquipmentComponent()->GetCurrentRangeWeapon();
	if (!IsValid(CurrentRangeWeapon))
	{
		return;
	}

	bIsAiming = true;
	CurrentAimingMovementSpeed = CurrentRangeWeapon->GetAimMovementMaxSpeed();
	CurrentRangeWeapon->StartAim();
	OnStartAiming();
}

void AGCBaseCharacter::StopAiming()
{
	if (!bIsAiming)
	{
		return;
	}

	ARangeWeaponItem* CurrentRangeWeapon = GetCharacterEquipmentComponent()->GetCurrentRangeWeapon();
	if (IsValid(CurrentRangeWeapon))
	{
		CurrentRangeWeapon->StopAim();
	}
	bIsAiming = false;
	CurrentAimingMovementSpeed = 0.0f;
	OnStopAiming();
}

bool AGCBaseCharacter::IsAiming() const
{
	return bIsAiming;
}

void AGCBaseCharacter::Reload()
{
	if (IsValid(CharacterEquipmentComponent->GetCurrentRangeWeapon()))
	{
		CharacterEquipmentComponent->ReloadCurrentWeapon();
	}
}

void AGCBaseCharacter::NextItem()
{
	CharacterEquipmentComponent->EquipNextItem();
}

void AGCBaseCharacter::PreviousItem()
{
	CharacterEquipmentComponent->EquipPreviousItem();
}

void AGCBaseCharacter::EquipPrimaryItem()
{
	CharacterEquipmentComponent->EquipItemInSlot(EEquipmentSlots::PrimaryItemSlot);
}

void AGCBaseCharacter::ChangeFireMode()
{
	CharacterEquipmentComponent->ChangeFireMode();
}

void AGCBaseCharacter::OnStartAiming_Implementation()
{
	OnStartAimingInternal();
}

void AGCBaseCharacter::OnStopAiming_Implementation()
{
	OnStopAimingInternal();
}

float AGCBaseCharacter::GetAimingMovementSpeed() const
{
	return CurrentAimingMovementSpeed;
}

const UCharacterEquipmentComponent* AGCBaseCharacter::GetCharacterEquipmentComponent() const
{
	return CharacterEquipmentComponent;
}

UCharacterEquipmentComponent* AGCBaseCharacter::GetCharacterEquipmentComponent_Mutable() const
{
	return CharacterEquipmentComponent;
}

const UCharacterAttributesComponent* AGCBaseCharacter::GetCharacterAttributesComponent() const
{
	return CharacterAttributesComponent;
}

bool AGCBaseCharacter::CanSprint()
{
	return GetMovementComponent()->IsMovingOnGround();
}

bool AGCBaseCharacter::CanProne()
{
	return bIsCrouched;
}

void AGCBaseCharacter::Prone()
{
	if (GCBaseCharacterMovementComponent)
	{
		if (CanProne())
		{
			GCBaseCharacterMovementComponent->SetIsProning(true);
			GCBaseCharacterMovementComponent->StartProne();
		}
	}
}

void AGCBaseCharacter::UnProne()
{
	if (GCBaseCharacterMovementComponent)
	{
		GCBaseCharacterMovementComponent->SetIsProning(false);
		GCBaseCharacterMovementComponent->StopProne();
	}
}

float AGCBaseCharacter::CalculatePelvisOffset()
{
	return -FMath::Abs(IKRightFootOffset - IKLeftFootOffset);
}

bool AGCBaseCharacter::CanMantle() const
{
	GetBaseCharacterMovementComponent()->IsOnZipline();
	return !GetBaseCharacterMovementComponent()->IsOnLadder() && !GetBaseCharacterMovementComponent()->IsOnZipline() && !(GetBaseCharacterMovementComponent()->MovementMode == MOVE_Falling);
}

void AGCBaseCharacter::OnMantle(const FMantlingSettings& MantlingSettings, float MantlingAnimationStartTime)
{

}

void AGCBaseCharacter::OnDeath()
{
	GetCharacterMovement()->DisableMovement();//MoveMode_NONE
	float Duration = PlayAnimMontage(OnDeathAnimMontage);
	if (Duration == 0.0f)
	{
		EnableRagdoll();
	}
}

void AGCBaseCharacter::OnStartAimingInternal()
{
	if (OnAimingStateChanged.IsBound())
	{
		OnAimingStateChanged.Broadcast(true);
	}
}

void AGCBaseCharacter::OnStopAimingInternal()
{
	if (OnAimingStateChanged.IsBound())
	{
		OnAimingStateChanged.Broadcast(false);
	}
}

void AGCBaseCharacter::TryChangeSprintState(float DeltaTime)
{
	if (bIsSprintRequested && !GCBaseCharacterMovementComponent->IsSprinting() && CanSprint() && !GCBaseCharacterMovementComponent->IsProning())
	{
		GCBaseCharacterMovementComponent->StartSprint();
		OnSprintStart();
	}
	if (!bIsSprintRequested && GCBaseCharacterMovementComponent->IsSprinting())
	{
		GCBaseCharacterMovementComponent->StopSprint();
		OnSprintEnd();
	}
}

const FMantlingSettings& AGCBaseCharacter::GetMantlingSettings(float LedgeHeight) const
{
	return LedgeHeight > LowMantleMaxHeight ? HighMantleSettings : LowMantleSettings;
}

//homework
float AGCBaseCharacter::GetIKOffsetForASocket(const FName& SocketName)
{
	float Result = 0.0f;
	FVector SocketLocation = GetMesh()->GetSocketLocation(SocketName);
	//New
	float CapsuleHalfHeight = GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
	//Old
	FVector TraceStart(SocketLocation.X, SocketLocation.Y, GetActorLocation().Z);
	FVector TraceEnd = TraceStart - (CapsuleHalfHeight + IKTraceDistance) * FVector::UpVector;

	FHitResult HitResult;
	ETraceTypeQuery TraceType = UEngineTypes::ConvertToTraceType(ECC_Visibility);
	if (UKismetSystemLibrary::LineTraceSingle(GetWorld(), TraceStart, TraceEnd, TraceType, true, TArray<AActor*>(), EDrawDebugTrace::ForOneFrame, HitResult, true))
	{
		float CharacterBottom = TraceStart.Z - CapsuleHalfHeight;
		Result = CharacterBottom - HitResult.Location.Z;
	}

	return Result;
}

void AGCBaseCharacter::EnableRagdoll()
{
	GetMesh()->SetCollisionProfileName(CollisionProfileRagdoll);
	GetMesh()->SetSimulatePhysics(true);
}

