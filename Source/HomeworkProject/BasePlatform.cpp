// Fill out your copyright notice in the Description page of Project Settings.

#include "BasePlatform.h"
#include "Kismet/GameplayStatics.h"
#include "GCBaseCharacter.h"
#include "PlatformInvocator.h"

// Sets default values
ABasePlatform::ABasePlatform()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	BoxSize = FVector(150.0f, 150.0f, 10.0f);
	USceneComponent* DefaultPlatformRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Platform root"));
	PrimitiveComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("PrimitiveComponent"));
	RootComponent = DefaultPlatformRoot;
	PrimitiveComponent->SetBoxExtent(BoxSize);
	PrimitiveComponent->SetCollisionProfileName("Trigger");
	PlatformMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Platform"));
	PlatformMesh->SetupAttachment(DefaultPlatformRoot);
	PrimitiveComponent->SetupAttachment(PlatformMesh);
}

// Called when the game starts or when spawned
void ABasePlatform::BeginPlay()
{
	Super::BeginPlay();
	/*Начальная позиция платформы.Сохраняется в методе BeginPlay() следующим образом :*/
	StartLocation = PlatformMesh->GetRelativeLocation();
	Character = Cast<AGCBaseCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (IsValid(TimelineCurve))
	{
		FOnTimelineFloatStatic PlatformMovementTimelineUpdate;
		
		PlatformMovementTimelineUpdate.BindUObject(this, &ABasePlatform::PlatformTimelineUpdate);
		PlatformTimeline.AddInterpFloat(TimelineCurve, PlatformMovementTimelineUpdate);
	}

	else if (IsValid(PlatformInvocator))
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue, FString(TEXT("IsValid")));
		//Здесь должна производиться подписка делегат
		PlatformInvocator->OnInvocatorActivated.AddDynamic(this, &ABasePlatform::OnPlatformInvoked);
	}
}
//вызывается только при OnDemand
void ABasePlatform::PlatformTimelineUpdate(const float Alpha)
{
	const FVector PlatformTargetLocation = FMath::Lerp(StartLocation, EndLocation, Alpha);
	
	PlatformMesh->SetRelativeLocation(PlatformTargetLocation);
}
//здесь логика
void ABasePlatform::DoIfChoosePlatformBehavior()
{
	if (PlatformBehavior == EPlatformBehavior::OnDemand)
	{
		PlatformTimeline.Play();
	}
	else if (PlatformBehavior == EPlatformBehavior::Loop)
	{
		auto location = PlatformMesh->GetRelativeLocation();
		if (location.Equals(EndLocation)) {
			FTimerHandle handle;
			GetWorld()->GetTimerManager().SetTimer(handle, [&]() { PlatformTimeline.Reverse(); }, SleepTime, false);
		}
		else if (location.Equals(StartLocation))
		{
			FTimerHandle handle;
			GetWorld()->GetTimerManager().SetTimer(handle, [&]() { PlatformTimeline.Play(); }, SleepTime, false);
		}
	}
}

void ABasePlatform::DoIfChoosePlatformInvocator()
{
	auto location = PlatformMesh->GetRelativeLocation();
	if (location.Equals(EndLocation)) {
		FTimerHandle handle;
		GetWorld()->GetTimerManager().SetTimer(handle, [&]() { PlatformTimeline.Reverse(); }, SleepTime, false);
	}
}

void ABasePlatform::OnPlatformInvoked()
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString(TEXT("Inside")));
	auto location = PlatformMesh->GetRelativeLocation();
	if (location.Equals(StartLocation))
	{
		PlatformTimeline.Play();
	}
}

// Called every frame
void ABasePlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	PlatformTimeline.TickTimeline(DeltaTime);
	if (IsValid(PlatformInvocator))
	{
		DoIfChoosePlatformInvocator();
	}
	else if (!IsValid(PlatformInvocator))
	{
		DoIfChoosePlatformBehavior();
	}

	if (IsValid(Character))
	{
		PlatformLocation(Character->GetActorLocation(), Character->GetActorQuat(), Character->GetMesh()->GetCollisionShape());
	}
}

void ABasePlatform::PlatformLocation(FVector Location, FQuat Rotation, FCollisionShape Collision)
{
	if (PrimitiveComponent->OverlapComponent(Location, Rotation, Collision))
	{
		Character->GetBaseCharacterMovementComponent()->IsNextToPlatform = 1;
		IsNextToPlatform = 1;
	}
	if (IsNextToPlatform==1){
		FVector OverlapLoc = GetActorLocation() + PlatformMesh->GetRelativeLocation();
		Character->GetBaseCharacterMovementComponent()->SetPlatformLocation(OverlapLoc);
	}
	if (Character->GetBaseCharacterMovementComponent()->IsNextToPlatform==0 || !Character->GetBaseCharacterMovementComponent()->IsMantling())
	{
		IsNextToPlatform = 0;
	}
}

void ABasePlatform::AddIngnorePlatfromCollision(FCollisionQueryParams QueryParams)
{
	QueryParams.AddIgnoredComponent(PrimitiveComponent);
}

