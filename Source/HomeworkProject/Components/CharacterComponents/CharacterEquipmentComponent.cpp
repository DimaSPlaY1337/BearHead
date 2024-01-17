// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/CharacterComponents/CharacterEquipmentComponent.h"
#include "GCBaseCharacter.h"
#include "Actors/Equipment/Throwables/ThrowableItem.h"
#include "Actors/Equipment/Weapons/RangeWeaponItem.h"
#include "GameCodeTypes.h"


EEquippedItemType UCharacterEquipmentComponent::GetCurrentEquippedItemType() const
{
	EEquippedItemType Result = EEquippedItemType::None;
	if (IsValid(CurrentEquippedItem))
	{
		Result = CurrentEquippedItem->GetItemType();
	}
	return Result;
}

ARangeWeaponItem* UCharacterEquipmentComponent::GetCurrentRangeWeapon() const
{
	return CurrentEquippedWeapon;
}

AMeleeWeaponItem* UCharacterEquipmentComponent::GetCurrentMeleeWeapon() const
{
	return CurrentMeleeWeapon;
}

void UCharacterEquipmentComponent::ReloadCurrentWeapon()
{
	check(IsValid(CurrentEquippedWeapon));
	int32 AvailableAmunition = GetAvailableAmunitionForCurrentWeapon();
	if (AvailableAmunition <= 0)
	{
		return;
	}

	CurrentEquippedWeapon->StartReload();
}

//экипируем текущее оружие предыдущее удаляем
void UCharacterEquipmentComponent::EquipItemInSlot(EEquipmentSlots Slot)
{
	if (bIsEquipping)
	{
		return;
	}

	UnEquipCurrentItem();
	InitialiseCurrentObjects(Slot);
	CurrentMeleeWeapon = Cast<AMeleeWeaponItem>(CurrentEquippedItem);

	if (IsValid(CurrentThrowableItem))
	{
		if (CurrentThrowableItem->GetItemsAmount() == 0)
		{
			return;
		}
	}

	if (IsValid(CurrentEquippedItem))
	{
		UAnimMontage* EquipMontage = CurrentEquippedItem->GetCharacterEquipAnimMontage();
		if (IsValid(EquipMontage))
		{
			bIsEquipping = true;
			float EquipDuration = CachedBaseCharacter->PlayAnimMontage(EquipMontage);
			GetWorld()->GetTimerManager().SetTimer(EquipTimer, this, &UCharacterEquipmentComponent::EquipAnimationFinished, EquipDuration, false);
		}
		else
		{
			AttachCurrentItemToEquippedSocket();
		}
		CurrentEquippedSlot = Slot;
		CurrentEquippedItem->Equip();
	}

	if (IsValid(CurrentEquippedWeapon))
	{
		CurrentAmunitionType = CurrentEquippedWeapon->GetAmmoType();
		OnCurrentWeaponAmmoChangedHandle = CurrentEquippedWeapon->OnAmmoChanged.AddUFunction(this, FName("OnCurrentWeaponAmmoChanged"));
		OnCurrentWeaponReloadedHandle = CurrentEquippedWeapon->OnReloadComplete.AddUFunction(this, FName("OnWeaponReloadComplete"));
		if (GetCurrentRangeWeapon()->AvaliableOptionalAmmo && IsAmmoChanged)
		{
			OnCurrentWeaponAmmoChanged(CurrentEquippedWeapon->GetOptionalAmmo());
		}
		else
		{
			OnCurrentWeaponAmmoChanged(CurrentEquippedWeapon->GetAmmo());
		}
	}

	if (IsValid(CurrentThrowableItem))
	{
		CurrentThrowableItem->OnThrowableItemsAmountChanged.AddUObject(this, &UCharacterEquipmentComponent::OnCurrentThrowableItemsAmountChanged);
	}

	if (OnEquippedItemChanged.IsBound())
	{
		OnEquippedItemChanged.Broadcast(CurrentEquippedItem);
	}
}

void UCharacterEquipmentComponent::EquipNextItem()
{
	uint32 CurrentSlotIndex = (uint32)CurrentEquippedSlot;
	uint32 NextSlotIndex = NextItemsArraySlotIndex(CurrentSlotIndex);

	while (CurrentSlotIndex != NextSlotIndex 
		&& IgnoreSlotsWhileSwitching.Contains((EEquipmentSlots)NextSlotIndex)
		&& IsValid(ItemsArray[NextSlotIndex]))
	{
		NextSlotIndex = NextItemsArraySlotIndex(NextSlotIndex);
	}
	if (CurrentSlotIndex != NextSlotIndex)
	{
		EquipItemInSlot((EEquipmentSlots)NextSlotIndex);
	}
}

void UCharacterEquipmentComponent::EquipPreviousItem()
{
	uint32 CurrentSlotIndex = (uint32)CurrentEquippedSlot;
	uint32 PreviousSlotIndex = PreviousItemsArraySlotIndex(CurrentSlotIndex);

	while (CurrentSlotIndex != PreviousSlotIndex 
		&& IgnoreSlotsWhileSwitching.Contains((EEquipmentSlots)PreviousSlotIndex)
		&& IsValid(ItemsArray[PreviousSlotIndex]))
	{
		PreviousSlotIndex = PreviousItemsArraySlotIndex(PreviousSlotIndex);
	}
	if (CurrentSlotIndex != PreviousSlotIndex)
	{
		EquipItemInSlot((EEquipmentSlots)PreviousSlotIndex);
	}
}

void UCharacterEquipmentComponent::BeginPlay()
{
	Super::BeginPlay();
	checkf(GetOwner()->IsA<AGCBaseCharacter>(), TEXT("can be used only with a BaseCharacter"));
	CachedBaseCharacter = StaticCast<AGCBaseCharacter*>(GetOwner());//StaticCast т.к мы знаем что работаем только с 1 объектом
	CreateLoadout();
	AutoEquip();
}

void UCharacterEquipmentComponent::UnEquipCurrentItem()
{
	if (IsValid(CurrentEquippedItem))
	{
		CurrentEquippedItem->AttachToComponent(CachedBaseCharacter->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, CurrentEquippedItem->GetUnEquippedSocketName());
		CurrentEquippedItem->UnEquip();
	}
	if (IsValid(CurrentEquippedWeapon))
	{
		CurrentEquippedWeapon->StopFire();
		CurrentEquippedWeapon->EndReload(false);
		CurrentEquippedWeapon->OnAmmoChanged.Remove(OnCurrentWeaponAmmoChangedHandle);
		CurrentEquippedWeapon->OnReloadComplete.Remove(OnCurrentWeaponReloadedHandle);
	}
	PreviousEquippedSlot = CurrentEquippedSlot;
	CurrentEquippedSlot = EEquipmentSlots::None;
}

void UCharacterEquipmentComponent::AttachCurrentItemToEquippedSocket()
{
	if (IsValid(CurrentEquippedItem))
	{
		CurrentEquippedItem->AttachToComponent(CachedBaseCharacter->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, CurrentEquippedItem->GetEquippedSocketName());
	}
}

void UCharacterEquipmentComponent::LaunchCurrentThrowableItem()
{
	if (CurrentThrowableItem)
	{
		CurrentThrowableItem->Throw();
		bIsEquipping = false;
		int32 ItemsAmount = CurrentThrowableItem->GetItemsAmount();
		ItemsAmount -= 1;
		OnCurrentThrowableItemsAmountChanged(ItemsAmount);
		CurrentThrowableItem->SetItemsAmount(ItemsAmount);
		EquipItemInSlot(PreviousEquippedSlot);
	}
}

bool UCharacterEquipmentComponent::CanChangeAmmoType() const
{
	if (CurrentEquippedItem->GetItemType() == EEquippedItemType::Rifle && CurrentEquippedWeapon->AvaliableOptionalAmmo)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void UCharacterEquipmentComponent::ChangeFireMode()
{
	if (CanChangeAmmoType())
	{
		CurrentEquippedWeapon->ChangeAmmoType();
		if (OnCurrentWeaponAmmoChangedEvent.IsBound())
		{
			if (!IsAmmoChanged)
			{
				CurrentEquippedWeapon->SetCurrentSlotsType(EAmunitionType::RifleGranades);
				CurrentAmunitionType = EAmunitionType::RifleGranades;
				uint32 Ammo = GetCurrentRangeWeapon()->GetOptionalAmmo();
				int32 ClipAmount = *MaxAmunitionAmount.Find(EAmunitionType::RifleGranades);
				OnCurrentWeaponAmmoChangedEvent.Broadcast(Ammo, ClipAmount);
				CurrentEquippedWeapon->SetOptionalAmmo(Ammo);
				IsAmmoChanged = true;
			}
			else
			{
				CurrentEquippedWeapon->SetCurrentSlotsType(EAmunitionType::Rifle);
				CurrentAmunitionType = EAmunitionType::Rifle;
				uint32 Ammo = GetCurrentRangeWeapon()->GetAmmo();
				int32 ClipAmount = *MaxAmunitionAmount.Find(EAmunitionType::Rifle);
				OnCurrentWeaponAmmoChangedEvent.Broadcast(Ammo, ClipAmount);
				CurrentEquippedWeapon->SetAmmo(Ammo);
				IsAmmoChanged = false;
			}
		}
	}
}

bool UCharacterEquipmentComponent::IsEquipping() const
{
	return bIsEquipping;
}

//текущее количество доступной амуниции
int32 UCharacterEquipmentComponent::GetAvailableAmunitionForCurrentWeapon()
{
	check(GetCurrentRangeWeapon());
	return AmunitionArray[(int32)GetCurrentRangeWeapon()->GetAmmoType()];
}

void UCharacterEquipmentComponent::AutoEquip()
{
	if (AutoEquipItemInSlot != EEquipmentSlots::None)
	{
		EquipItemInSlot(AutoEquipItemInSlot);
	}
}

void UCharacterEquipmentComponent::InitialiseCurrentObjects(EEquipmentSlots Slot)
{
	CurrentEquippedItem = ItemsArray[(uint32)Slot];
	CurrentEquippedWeapon = Cast<ARangeWeaponItem>(CurrentEquippedItem);
	CurrentThrowableItem = Cast<AThrowableItem>(CurrentEquippedItem);
}

//разгрузка персонажа. Базовые предметы которые есть у персонажа
//работает не с одним свойством а с целым массивом
void UCharacterEquipmentComponent::CreateLoadout()
{
	AmunitionArray.AddZeroed((uint32)EAmunitionType::MAX);
	for (const TPair<EAmunitionType, int32>& AmmoPair : MaxAmunitionAmount)
	{
		AmunitionArray[(uint32)AmmoPair.Key] = FMath::Max(AmmoPair.Value, 0);//защита от дурака, чтобы дизайнеры не вводили отрицательные значения
	}

	ItemsArray.AddZeroed((uint32)EEquipmentSlots::MAX);
	for (const TPair<EEquipmentSlots, TSubclassOf<AEquipableItem>>& ItemPair : ItemsLoadout)
	{
		if (!IsValid(ItemPair.Value))//проверка, заполнен ли
		{
			continue;
		}
		AEquipableItem* Item = GetWorld()->SpawnActor<AEquipableItem>(ItemPair.Value);//спавним эктора item-а
		Item->AttachToComponent(CachedBaseCharacter->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, Item->GetUnEquippedSocketName());
		Item->UnEquip();
		Item->SetOwner(CachedBaseCharacter.Get());//задаём владельца item-а
		ItemsArray[(uint32)ItemPair.Key] = Item;
	}
	InitialiseCurrentObjects(EEquipmentSlots::PrimaryItemSlot);
	if (IsValid(CurrentEquippedItem))
	{
		OnCurrentThrowableItemsAmountChanged(CurrentThrowableItem->GetMaxItemsAmount());
	}
}

void UCharacterEquipmentComponent::EquipAnimationFinished()
{
	bIsEquipping = false;
	AttachCurrentItemToEquippedSocket();
}

uint32 UCharacterEquipmentComponent::NextItemsArraySlotIndex(uint32 CurrentSlotIndex)
{
	if (CurrentSlotIndex == ItemsArray.Num() - 1)
	{
		return 0;
	}
	else
	{
		return CurrentSlotIndex + 1;
	}
}

uint32 UCharacterEquipmentComponent::PreviousItemsArraySlotIndex(uint32 CurrentSlotIndex)
{
	if (CurrentSlotIndex == 0)
	{
		return ItemsArray.Num() - 1;
	}
	else
	{
		return CurrentSlotIndex - 1;
	}
}

void UCharacterEquipmentComponent::ReloadAmmoInCurrentWeapon(int32 NumberOfAmmo, bool bCheckIsFull)
{
	int32 AvailableAmunition = GetAvailableAmunitionForCurrentWeapon();
	int32 CurrentAmmo;
	int32 AmmoToReload;
	if (IsAmmoChanged)
	{
		CurrentAmmo = CurrentEquippedWeapon->GetOptionalAmmo();
		AmmoToReload = CurrentEquippedWeapon->GetMaxOptionalAmmo() - CurrentEquippedWeapon->GetOptionalAmmo();
	}
	else
	{
		CurrentAmmo = CurrentEquippedWeapon->GetAmmo();
		AmmoToReload = CurrentEquippedWeapon->GetMaxAmmo() - CurrentEquippedWeapon->GetAmmo();
	}

	int32 ReloadedAmmo = FMath::Min(AvailableAmunition, AmmoToReload);
	if (NumberOfAmmo > 0)
	{
		ReloadedAmmo = FMath::Min(ReloadedAmmo, NumberOfAmmo);
	}

	AmunitionArray[(uint32)GetCurrentRangeWeapon()->GetAmmoType()] -= ReloadedAmmo;

	if (IsAmmoChanged)
	{
		CurrentEquippedWeapon->SetOptionalAmmo(ReloadedAmmo + CurrentAmmo);
	}
	else
	{
		CurrentEquippedWeapon->SetAmmo(ReloadedAmmo + CurrentAmmo);
	}

	if (bCheckIsFull)
	{
		AvailableAmunition = AmunitionArray[(uint32)CurrentEquippedWeapon->GetAmmoType()];
		bool bIsFullyReloaded = CurrentEquippedWeapon->GetAmmo() == CurrentEquippedWeapon->GetMaxAmmo();
		if (AvailableAmunition == 0 || bIsFullyReloaded)
		{
			CurrentEquippedWeapon->EndReload(true);
		}
	}	
}

void UCharacterEquipmentComponent::OnWeaponReloadComplete()
{
	ReloadAmmoInCurrentWeapon(0, false);
}

void UCharacterEquipmentComponent::OnCurrentWeaponAmmoChanged(int32 Ammo)
{
	if (OnCurrentWeaponAmmoChangedEvent.IsBound())
	{
		OnCurrentWeaponAmmoChangedEvent.Broadcast(Ammo, GetAvailableAmunitionForCurrentWeapon());
	}
	if (GetCurrentRangeWeapon()->AvaliableOptionalAmmo && Ammo == 0)
	{
		ReloadCurrentWeapon();
	}
}

void UCharacterEquipmentComponent::OnCurrentThrowableItemsAmountChanged(int32 Items)
{
	if (OnCurrentThrowableItemsAmountChangedEvent.IsBound())
	{
		OnCurrentThrowableItemsAmountChangedEvent.Broadcast(Items);
	}
}
