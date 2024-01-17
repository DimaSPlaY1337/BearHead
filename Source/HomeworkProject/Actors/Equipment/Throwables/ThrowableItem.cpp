// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Equipment/Throwables/ThrowableItem.h"
#include "GCBaseCharacter.h"
#include "Actors/Projectiles/GCProjectile.h"

void AThrowableItem::Throw()
{
	AGCBaseCharacter* CharacterOwner = GetCharacterOwner();
	if (!IsValid(CharacterOwner))
	{
		return;
	}

	APlayerController* Controller = CharacterOwner->GetController<APlayerController>();
	if (!IsValid(Controller))
	{
		return;
	}

	FVector PlayerViewPoint;
	FRotator PlayerViewRotation;

	Controller->GetPlayerViewPoint(PlayerViewPoint, PlayerViewRotation);
	FTransform PlayerViewTransform(PlayerViewRotation, PlayerViewPoint);

	FVector ViewDirection = PlayerViewRotation.RotateVector(FVector::ForwardVector);
	FVector ViewUpVector = PlayerViewRotation.RotateVector(FVector::UpVector);

	FVector LaunchDirection = ViewDirection + FMath::Tan(FMath::DegreesToRadians(ThrowableAngle)) * ViewUpVector;
	FVector ThrowableSocketLocation = CharacterOwner->GetMesh()->GetSocketLocation(SocketCharacterThrowable);
	//перевод из мировых координат в локальные
	FVector SocketInViewSpace = PlayerViewTransform.InverseTransformPosition(ThrowableSocketLocation);

	FVector SpawnLocation = PlayerViewPoint + ViewDirection * SocketInViewSpace.X;
	AGCProjectile* Projectile = GetWorld()->SpawnActor<AGCProjectile>(ProjectileClass, SpawnLocation, FRotator::ZeroRotator);
	if (IsValid(Projectile))
	{
		Projectile->SetOwner(GetOwner());
		Projectile->LaunchProjectile(LaunchDirection.GetSafeNormal());
	}
}

int32 AThrowableItem::GetMaxItemsAmount() const
{
	return MaxItemsAmount;
}

int32 AThrowableItem::GetItemsAmount() const
{
	return Items;
}

void AThrowableItem::SetItemsAmount(int32 NewItems)
{
	Items = NewItems;
	if (OnThrowableItemsAmountChanged.IsBound())
	{
		OnThrowableItemsAmountChanged.Broadcast(NewItems);
	}
}

void AThrowableItem::BeginPlay()
{
	Super::BeginPlay();
	SetItemsAmount(MaxItemsAmount);
	UE_LOG(LogTemp, Warning, TEXT("%d"), MaxItemsAmount);
}
