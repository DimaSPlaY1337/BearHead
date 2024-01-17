// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameCodeTypes.h"
#include "EquipableItem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEquipmentStateChanged, bool, bIsEquipped);

class UAnimMontage;
class AGCBaseCharacter;
UCLASS(Abstract, NotBlueprintable)
class HOMEWORKPROJECT_API AEquipableItem : public AActor
{
	GENERATED_BODY()

public:
	virtual void SetOwner(AActor* NewOwner) override;

	EEquippedItemType GetItemType() const { return ItemType; }

	UAnimMontage* GetCharacterEquipAnimMontage() const;

	FName GetUnEquippedSocketName() const;
	FName GetEquippedSocketName() const;

	virtual void Equip();
	virtual void UnEquip();

	virtual EReticleType GetReticleType() const;

protected:
	UPROPERTY(BlueprintAssignable)
		FOnEquipmentStateChanged OnEquipmentStateChanged;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Equipable item")
	EEquippedItemType ItemType = EEquippedItemType::None;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Equipable item")
	UAnimMontage* CharacterEquipAnimMontage;

	//им€ сокета в тот момент когда item не прикреплЄн ни к чему
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Equipable item")
	FName UnEquippedSocketName = NAME_None;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Equipable item")
	FName EquippedSocketName = NAME_None;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Reticle")
		EReticleType ReticleType = EReticleType::None;

	AGCBaseCharacter* GetCharacterOwner() const;

private:
	TWeakObjectPtr<AGCBaseCharacter> CachedCharacterOwner;
};
