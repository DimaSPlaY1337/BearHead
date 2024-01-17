// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameCodeTypes.h"
#include "Components/Weapon/WeaponBarellComponent.h"
#include "CharacterEquipmentComponent.generated.h"

typedef TArray<int32, TInlineAllocator<(uint32)EAmunitionType::MAX>> TAmunitionArray;
//колл которая хранит заспавненный loadout, который уже создан
typedef TArray<class AEquipableItem*, TInlineAllocator<(uint32)EEquipmentSlots::MAX>> TItemsArray;

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnCurrentWeaponAmmoChanged, int32, int32);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnCurrentThrowableItemsAmountChanged, int32);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnEquippedItemChanged, const AEquipableItem*);

class AThrowableItem;
class ARangeWeaponItem;
class AMeleeWeaponItem;
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class HOMEWORKPROJECT_API UCharacterEquipmentComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	EEquippedItemType GetCurrentEquippedItemType() const;

	ARangeWeaponItem* GetCurrentRangeWeapon() const;

	AMeleeWeaponItem* GetCurrentMeleeWeapon() const;

	void ReloadCurrentWeapon();

	FOnCurrentWeaponAmmoChanged OnCurrentWeaponAmmoChangedEvent;
	FOnCurrentThrowableItemsAmountChanged OnCurrentThrowableItemsAmountChangedEvent;
	FOnEquippedItemChanged OnEquippedItemChanged;

	void EquipItemInSlot(EEquipmentSlots Slot);

	void EquipNextItem();
	void EquipPreviousItem();

	void AttachCurrentItemToEquippedSocket();

	void LaunchCurrentThrowableItem();

	bool CanChangeAmmoType() const;

	void ChangeFireMode();

	bool IsEquipping() const;

	void ReloadAmmoInCurrentWeapon(int32 NumberOfAmmo = 0, bool bCheckIsFull = false);

protected:
	virtual void BeginPlay() override;

	void UnEquipCurrentItem();
	//кол-во обойм
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Loadout")
		TMap<EAmunitionType, int32> MaxAmunitionAmount;
	//коллекция разгрузки по оружию
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Loadout")
		TMap<EEquipmentSlots, TSubclassOf<class AEquipableItem>> ItemsLoadout;
	//коллекция которая говорит какие слоты мы должны игнорировать при переключении
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Loadout")
		TSet<EEquipmentSlots> IgnoreSlotsWhileSwitching;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Loadout")
		EEquipmentSlots AutoEquipItemInSlot = EEquipmentSlots::None;

private:
	TItemsArray ItemsArray;
	TAmunitionArray AmunitionArray;

	int32 GetAvailableAmunitionForCurrentWeapon();

	void AutoEquip();

	void InitialiseCurrentObjects(EEquipmentSlots Slot);

	void CreateLoadout();

	void EquipAnimationFinished();

	uint32 NextItemsArraySlotIndex(uint32 CurrentSlotIndex);
	uint32 PreviousItemsArraySlotIndex(uint32 CurrentSlotIndex);

	bool bIsEquipping;

	bool IsAmmoChanged;

	UFUNCTION()
		void OnWeaponReloadComplete();

	UFUNCTION()
		void OnCurrentWeaponAmmoChanged(int32 Ammo);

	void OnCurrentThrowableItemsAmountChanged(int32 Items);

	FDelegateHandle OnCurrentWeaponAmmoChangedHandle;
	FDelegateHandle OnCurrentWeaponReloadedHandle;

	EEquipmentSlots PreviousEquippedSlot;
	EEquipmentSlots CurrentEquippedSlot;

	EAmunitionType CurrentAmunitionType;

	AEquipableItem* CurrentEquippedItem;//хранит уже привиденный тип. Это съэкономит на кастах
	ARangeWeaponItem* CurrentEquippedWeapon;
	AThrowableItem* CurrentThrowableItem;
	AMeleeWeaponItem* CurrentMeleeWeapon;

	TWeakObjectPtr<class AGCBaseCharacter> CachedBaseCharacter;

	FTimerHandle  EquipTimer;
};
