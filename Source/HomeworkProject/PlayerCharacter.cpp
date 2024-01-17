// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GCBaseCharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GCBaseCharacter.h"
#include "Actors/Equipment/Weapons/RangeWeaponItem.h"
#include "Components/CharacterComponents/CharacterEquipmentComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SkeletalMeshComponent.h"
#include "Camera/CameraComponent.h"
#include "Runtime/Engine/Classes/Kismet/KismetSystemLibrary.h"
#include "Animation/AnimInstance.h"

APlayerCharacter::APlayerCharacter(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer)
{
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring arm"));
	SpringArmComponent->SetupAttachment(RootComponent);
	SpringArmComponent->bUsePawnControlRotation = true;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComponent->SetupAttachment(SpringArmComponent);
	CameraComponent->bUsePawnControlRotation = false;

	GetCharacterMovement()->bOrientRotationToMovement = 1;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f);

	Team = ETeams::Player;
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	if (IsValid(AimCurve))
	{
		FOnTimelineFloatStatic AimTimelineUpdate;
		AimTimelineUpdate.BindUObject(this, &APlayerCharacter::AimCameraUpdate);
		AimingTimeline.AddInterpFloat(AimCurve, AimTimelineUpdate);
	}
}

void APlayerCharacter::MoveForward(float Value)
{
	ForwardAxis = Value;
	if ((GetCharacterMovement()->IsMovingOnGround() || GetCharacterMovement()->IsFalling()) && !FMath::IsNearlyZero(Value, 1e-6f))
	{
		FRotator YawRotator(0.0f, GetControlRotation().Yaw, 0.0f);
		FVector ForwardVector = YawRotator.RotateVector(FVector::ForwardVector);
		AddMovementInput(ForwardVector, Value);
	}
}

void APlayerCharacter::MoveRight(float Value)
{
	RightAxis = Value;
	if ((GetCharacterMovement()->IsMovingOnGround() || GetCharacterMovement()->IsFalling()) && !FMath::IsNearlyZero(Value, 1e-6f))
	{
		FRotator YawRotator(0.0f, GetControlRotation().Yaw, 0.0f);
		FVector RightVector = YawRotator.RotateVector(FVector::RightVector);
		AddMovementInput(RightVector, Value);
	}
}

void APlayerCharacter::Turn(float Value)
{
	if (IsAiming)
	{
		ARangeWeaponItem* CurrentRangeWeapon = CharacterEquipmentComponent->GetCurrentRangeWeapon();
		AddControllerYawInput(Value * CurrentRangeWeapon->GetAimLookUpModifier());
	}
	else
	{
		AddControllerYawInput(Value);
	}
}

void APlayerCharacter::TurnAtRate(float Value)
{
	if (IsAiming)
	{
		ARangeWeaponItem* CurrentRangeWeapon = CharacterEquipmentComponent->GetCurrentRangeWeapon();
		AddControllerYawInput(Value * CurrentRangeWeapon->GetAimTurnModifier() * BaseTurnRate * GetWorld()->GetDeltaSeconds());
	}
	else
	{
		AddControllerYawInput(Value * BaseTurnRate * GetWorld()->GetDeltaSeconds());
	}
}

void APlayerCharacter::LookUp(float Value)
{
	if (IsAiming)
	{
		ARangeWeaponItem* CurrentRangeWeapon = CharacterEquipmentComponent->GetCurrentRangeWeapon();
		AddControllerPitchInput(Value * CurrentRangeWeapon->GetAimLookUpModifier());
	}
	else
	{
		AddControllerPitchInput(Value);
	}
}

void APlayerCharacter::LookUpAtRate(float Value)
{
	if (IsAiming)
	{
		ARangeWeaponItem* CurrentRangeWeapon = CharacterEquipmentComponent->GetCurrentRangeWeapon();
		AddControllerPitchInput(Value * CurrentRangeWeapon->GetAimLookUpModifier() * BaseTurnRate * GetWorld()->GetDeltaSeconds());
	}
	else
	{
		AddControllerPitchInput(Value * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
	}
}

void APlayerCharacter::OnStartCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust)
{
	Super::OnStartCrouch(HalfHeightAdjust, ScaledHalfHeightAdjust);
	SpringArmComponent->TargetOffset += FVector(0.0f, 0.0f, HalfHeightAdjust);
}

void APlayerCharacter::OnEndCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust)
{
	Super::OnEndCrouch(HalfHeightAdjust, ScaledHalfHeightAdjust);
	SpringArmComponent->TargetOffset -= FVector(0.0f, 0.0f, HalfHeightAdjust);
}

bool APlayerCharacter::CanJumpInternal_Implementation() const
{
	return (bIsCrouched || Super::CanJumpInternal_Implementation()) && !GetBaseCharacterMovementComponent()->IsMantling();
}

void APlayerCharacter::OnJumped_Implementation()
{
	if (bIsCrouched)
	{
		UnCrouch();
	}
}

void APlayerCharacter::OnStartProne(float HalfHeightAdjust)
{
	SpringArmComponent->TargetOffset += FVector(0.0f, 0.0f, HalfHeightAdjust);
}

void APlayerCharacter::OnEndProne(float HalfHeightAdjust)
{
	SpringArmComponent->TargetOffset -= FVector(0.0f, 0.0f, HalfHeightAdjust);
}

void APlayerCharacter::OnStartSlide(float HalfHeightAdjust)
{
	SpringArmComponent->TargetOffset += FVector(0.0f, 0.0f, HalfHeightAdjust);
	FVector& MeshRelativeLocation = GetMesh()->GetRelativeLocation_DirectMutable();
	MeshRelativeLocation.Z = GetMesh()->GetRelativeLocation().Z + HalfHeightAdjust;
}

void APlayerCharacter::OnEndSlide(float HalfHeightAdjust)
{
	SpringArmComponent->TargetOffset -= FVector(0.0f, 0.0f, HalfHeightAdjust);
	const APlayerCharacter* DefaultCharacter = GetClass()->GetDefaultObject<APlayerCharacter>();
	GetMesh()->SetRelativeLocation(DefaultCharacter->GetMesh()->GetRelativeLocation());
}

void APlayerCharacter::SwimForward(float Value)
{
	if (GetCharacterMovement()->IsSwimming() && !FMath::IsNearlyZero(Value, 1e-6f))
	{
		FRotator PitchYawRotator(GetControlRotation().Pitch, GetControlRotation().Yaw, 0.0f);
		FVector ForwardVector = PitchYawRotator.RotateVector(FVector::ForwardVector);
		AddMovementInput(ForwardVector, Value);
	}
}

void APlayerCharacter::SwimRight(float Value)
{
	if (GetCharacterMovement()->IsSwimming() && !FMath::IsNearlyZero(Value, 1e-6f))
	{
		FRotator YawRotator(0.0f, GetControlRotation().Yaw, 0.0f);
		FVector RightVector = YawRotator.RotateVector(FVector::RightVector);
		AddMovementInput(RightVector, Value);
	}
}

void APlayerCharacter::SwimUp(float Value)
{
	if (GetCharacterMovement()->IsSwimming() && !FMath::IsNearlyZero(Value, 1e-6f))
	{
		AddMovementInput(FVector::UpVector, Value);
	}
}

void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (AimingTimeline.IsPlaying())
	{
		AimingTimeline.TickTimeline(DeltaTime);
	}
	//UE_LOG(LogTemp, Warning, TEXT("%f %f"), GetWorld()->GetDeltaSeconds(), DeltaTime);
}

void APlayerCharacter::OnStartAimingInternal()
{
	Super::OnStartAimingInternal();
	APlayerController* PlayerController = GetController<APlayerController>();
	if (!IsValid(PlayerController))
	{
		return;
	}

	APlayerCameraManager* CameraManager = PlayerController->PlayerCameraManager;
	if (IsValid(CameraManager))
	{
		AimingTimeline.Play();
	}
}

void APlayerCharacter::OnStopAimingInternal()
{
	Super::OnStopAimingInternal();
	APlayerController* PlayerController = GetController<APlayerController>();
	if (!IsValid(PlayerController))
	{
		return;
	}

	APlayerCameraManager* CameraManager = PlayerController->PlayerCameraManager;
	if (IsValid(CameraManager))
	{
		AimingTimeline.Reverse();
	}
}

void APlayerCharacter::AimCameraUpdate(float Value)
{
	APlayerController* PlayerController = GetController<APlayerController>();
	APlayerCameraManager* CameraManager = PlayerController->PlayerCameraManager;
	ARangeWeaponItem* CurrentRangeWeapon = CharacterEquipmentComponent->GetCurrentRangeWeapon();
	CameraManager->SetFOV(FMath::Lerp(CameraManager->DefaultFOV, CurrentRangeWeapon->GetAimFOV(), Value));
	SetIsAiming(Value);
}

void APlayerCharacter::SetIsAiming(float Value)
{
	if (int(Value*10)==0)
	{
		IsAiming = false;
	}
	else
	{
		IsAiming = true;
	}
}
