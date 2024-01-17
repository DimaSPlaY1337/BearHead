// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Equipment/EquipableItem.h"
#include "GCBaseCharacter.h"

void AEquipableItem::SetOwner(AActor* NewOwner)
{
    Super::SetOwner(NewOwner);
    if (IsValid(NewOwner))
    {
        checkf(GetOwner()->IsA<AGCBaseCharacter>(), TEXT("only character can be an owner of range weapon"));
        CachedCharacterOwner = StaticCast<AGCBaseCharacter*>(GetOwner());
    }
    else
    {
        CachedCharacterOwner = nullptr;
    }
}

FName AEquipableItem::GetUnEquippedSocketName() const
{
    return UnEquippedSocketName;
}

FName AEquipableItem::GetEquippedSocketName() const
{
    return EquippedSocketName;
}

UAnimMontage* AEquipableItem::GetCharacterEquipAnimMontage() const
{
    return CharacterEquipAnimMontage;
}

void AEquipableItem::Equip()
{
    if (OnEquipmentStateChanged.IsBound())
    {
        OnEquipmentStateChanged.Broadcast(true);
    }
}

void AEquipableItem::UnEquip()
{
    if (OnEquipmentStateChanged.IsBound())
    {
        OnEquipmentStateChanged.Broadcast(false);
    }
}

EReticleType AEquipableItem::GetReticleType() const
{
    return ReticleType;
}

AGCBaseCharacter* AEquipableItem::GetCharacterOwner() const
{
    return CachedCharacterOwner.IsValid() ? CachedCharacterOwner.Get() : nullptr;
}
