// Fill out your copyright notice in the Description page of Project Settings.


#include "Zipline.h"
#include "GameCodeTypes.h"
#include "Kismet/GameplayStatics.h"

AZipline::AZipline()
{
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("ZiplineRoot"));

	FirstPostComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FirstPost"));
	FirstPostComponent->SetupAttachment(RootComponent);

	SecondPostComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SecondPost"));
	SecondPostComponent->SetupAttachment(RootComponent);

	CableComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Cable"));
	CableComponent->SetupAttachment(RootComponent);

	InteractionVolume = CreateDefaultSubobject<UCapsuleComponent>(TEXT("InterectionVolume"));
	InteractionVolume->SetupAttachment(RootComponent);
	InteractionVolume->SetCollisionProfileName(CollisionProfilePawnInteractionVolume);
	InteractionVolume->SetGenerateOverlapEvents(true);
}

void AZipline::OnConstruction(const FTransform& Transform)
{
	FirstPostComponent->SetRelativeLocation(FirstPost - FVector(0.0f, 0.0f, ZiplineHeight * 0.5f));
	SecondPostComponent->SetRelativeLocation(SecondPost - FVector(0.0f, 0.0f, ZiplineHeight * 0.5f));
	//настройка кабеля
	Point1 = FirstPostComponent->GetRelativeLocation();
	Point2 = SecondPostComponent->GetRelativeLocation();
	FVector delta = (Point1 - Point2);
	FVector CableAndCapsuleLoc = (-delta) / 2 + Point1 + FVector(0.0f, 0.0f, ZiplineHeight * 0.5);
	FRotator CableRot = delta.ToOrientationRotator();
	FRotator CapsuleRot = FRotator(0.0f, CableRot.Yaw, CableRot.Pitch) + FRotator(0.0f, 90.0f, 90.0f);
	GetInterectionVolume()->SetRelativeLocation(CableAndCapsuleLoc);
	GetInterectionVolume()->SetRelativeRotation(CapsuleRot+FRotator(0.0f, 0.0f, 180.0f));
	CableRot = FRotator(CableRot.Pitch, CableRot.Yaw, 0.0f);
	CableComponent->SetRelativeLocation(CableAndCapsuleLoc);
	CableComponent->SetRelativeRotation(CableRot);

	UStaticMesh* FirstPostMesh = FirstPostComponent->GetStaticMesh();
	if (IsValid(FirstPostMesh))
	{
		float MeshHeight = FirstPostMesh->GetBoundingBox().GetSize().Z;
		if (!FMath::IsNearlyZero(MeshHeight))
		{
			FirstPostComponent->SetRelativeScale3D(FVector(1.0f, 1.0f, ZiplineHeight/MeshHeight));
		}
	}

	UStaticMesh* SecondPostMesh = SecondPostComponent->GetStaticMesh();
	if (IsValid(SecondPostMesh))
	{
		float MeshHeight = SecondPostMesh->GetBoundingBox().GetSize().Z;
		if (!FMath::IsNearlyZero(MeshHeight))
		{
			SecondPostComponent->SetRelativeScale3D(FVector(1.0f, 1.0f, ZiplineHeight/MeshHeight));
		}
	}
	
	UStaticMesh* CableMesh = CableComponent->GetStaticMesh();
	if (IsValid(CableMesh))
	{
		FVector CableMeshLenghtVector = CableMesh->GetBoundingBox().GetSize();
		float CableMeshAbsLenghtVector = sqrt(pow(CableMeshLenghtVector.X, 2) + pow(CableMeshLenghtVector.Y, 2) + pow(CableMeshLenghtVector.Z, 2));
		float LenghtFromP1ToP2 = sqrt(pow(Point1.X - Point2.X, 2) + pow(Point1.Y - Point2.Y, 2) + pow(Point1.Z - Point2.Z, 2));
		if (!FMath::IsNearlyZero(CableMeshAbsLenghtVector))
		{
			GetInterectionVolume()->SetCapsuleHalfHeight(LenghtFromP1ToP2/2);
			CableComponent->SetRelativeScale3D(FVector(LenghtFromP1ToP2 / CableMeshAbsLenghtVector, 1.0f, 1.0f));
		}
	}


}

void AZipline::BeginPlay()
{
	Super::BeginPlay();
	Point1 = FirstPostComponent->GetComponentLocation();
	Point2 = SecondPostComponent->GetComponentLocation();
}

void AZipline::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

UCapsuleComponent* AZipline::GetInterectionVolume() const
{
	return StaticCast<UCapsuleComponent*>(InteractionVolume);
}

void AZipline::OnInteractionVolumeOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnInteractionVolumeOverlapBegin(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	if (!IsOverlappingCharacterCapsule(OtherActor, OtherComp))
	{
		return;
	}
}

void AZipline::OnInteractionVolumeOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::OnInteractionVolumeOverlapEnd(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);

	if (!IsOverlappingCharacterCapsule(OtherActor, OtherComp))
	{
		return;
	}
}
