#include "Components/Weapon/MeleeHitRegistrator.h"
#include "GameCodeTypes.h"
#include "Utils/GCTraceUtils.h"

UMeleeHitRegistrator::UMeleeHitRegistrator()
{
	PrimaryComponentTick.bCanEverTick = true;
	SphereRadius = 5.0f;
	SetCollisionProfileName(CollisionProfileNoCollision);
}

void UMeleeHitRegistrator::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (bIsHitRegistrationEnabled)
	{
		ProcessHitRegistration();
	}
	PreviousComponentLocation = GetComponentLocation();
}

void UMeleeHitRegistrator::ProcessHitRegistration()
{
	FVector CurrentLocation = GetComponentLocation();
	FHitResult HitResult;

#if ENABLE_DRAW_DEBUG
	UDebugSubsystem* DebugSubSystem = UGameplayStatics::GetGameInstance(GetWorld())->GetSubsystem<UDebugSubsystem>();
	bool bIsDebugEnabled = DebugSubSystem->IsCategoryEnabled(DebugCategoryMeleeWeapon);
#else
	bool bIsDebugEnabled = false;
#endif

	bool bHasHit = GCTraceUtils::SweepSphereSingleByChanel(
		GetWorld(),
		HitResult,
		PreviousComponentLocation,
		CurrentLocation,
		GetScaledSphereRadius(),
		ECC_Melee,
		FCollisionQueryParams::DefaultQueryParam,
		FCollisionResponseParams::DefaultResponseParam,
		bIsDebugEnabled,
		5.0f
	);

	if (bHasHit)
	{
		FVector Direction = (CurrentLocation - PreviousComponentLocation).GetSafeNormal();
		if (OnMeleeHitRegistred.IsBound())
		{
			OnMeleeHitRegistred.Broadcast(HitResult, Direction);
		}
	}
}

void UMeleeHitRegistrator::SetIsHitRegistrationEnabled(bool bIsEnabled_In)
{
	bIsHitRegistrationEnabled = bIsEnabled_In;
}
