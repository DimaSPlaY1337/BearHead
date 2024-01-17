// Fill out your copyright notice in the Description page of Project Settings.

#include "GameCodeBasePawn.h"
#include "Components/SphereComponent.h"
#include "Engine/CollisionProfile.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "Templates/SharedPointer.h"
#include "GCBasePawnMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/PlayerCameraManager.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/ArrowComponent.h"
#include "HomeworkProject.h"

// Sets default values
AGameCodeBasePawn::AGameCodeBasePawn()
{
	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	CollisionComponent->SetSphereRadius(CollisionSphereRadius);
	CollisionComponent->SetCollisionProfileName(UCollisionProfile::Pawn_ProfileName);
	RootComponent = CollisionComponent;

	//MovementComponent = CreateDefaultSubobject<UPawnMovementComponent, UFloatingPawnMovement>(TEXT("Movement component"));
	MovementComponent = CreateDefaultSubobject<UPawnMovementComponent, UGCBasePawnMovementComponent>(TEXT("Movement component"));
	MovementComponent->SetUpdatedComponent(CollisionComponent);

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArmComponent->bUsePawnControlRotation = 1;
	SpringArmComponent->SetupAttachment(RootComponent);

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComponent->SetupAttachment(SpringArmComponent);

#if	WITH_EDITORONLY_DATA
	ArrowComponent = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
	ArrowComponent->SetupAttachment(RootComponent);
#endif
}

// Called to bind functionality to input
void AGameCodeBasePawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("MoveForward", this, &AGameCodeBasePawn::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AGameCodeBasePawn::MoveRight);
	PlayerInputComponent->BindAction("Jump", EInputEvent::IE_Pressed, this, &AGameCodeBasePawn::Jump);
}

void AGameCodeBasePawn::MoveForward(float value)
{
	InputForward = value;
	if (value != 0.0f)
	{
		AddMovementInput(CurrentViewActor->GetActorForwardVector(), value);
	}
}

void AGameCodeBasePawn::MoveRight(float value)
{
	InputRight = value;
	if (value != 0.0f)
	{
		AddMovementInput(CurrentViewActor->GetActorRightVector(), value);
	}
}

void AGameCodeBasePawn::Jump()
{
	checkf(MovementComponent->IsA<UGCBasePawnMovementComponent>(), TEXT("AGameCodeBasePawn::Jump() jump can work only with UGCBasePawnMovementComponent"));
	UGCBasePawnMovementComponent* BaseMovement = StaticCast<UGCBasePawnMovementComponent*>(MovementComponent);
	BaseMovement->JumpStart();
}

void AGameCodeBasePawn::BeginPlay()
{
	Super::BeginPlay();
	APlayerCameraManager* CameraManager = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);
	CurrentViewActor = CameraManager->GetViewTarget();
	CameraManager->OnBlendComplete().AddUFunction(this, FName("OnBlendComplete"));
}

void AGameCodeBasePawn::OnBlendComplete()
{
	CurrentViewActor = GetController()->GetViewTarget();
	UE_LOG(LogCameras, Verbose, TEXT("AGameCodeBasePawn::OnBlendComplete Current view actor: %s"), *CurrentViewActor->GetName());
}

