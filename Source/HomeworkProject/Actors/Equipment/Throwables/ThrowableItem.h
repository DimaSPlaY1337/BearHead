// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Equipment/EquipableItem.h"
#include "UI/Widget/PlayerHUDWidget.h"
#include "ThrowableItem.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(ThrowableItemsAmountChanged, int32);

UCLASS(Blueprintable)
class HOMEWORKPROJECT_API AThrowableItem : public AEquipableItem
{
	GENERATED_BODY()
	
public:
	void Throw();

	ThrowableItemsAmountChanged OnThrowableItemsAmountChanged;

	int32 GetMaxItemsAmount() const;

	int32 GetItemsAmount() const;

	void SetItemsAmount(int32 NewItems);

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Throwables")
		TSubclassOf<class AGCProjectile> ProjectileClass;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Throwables | Parametres | Items")
		int32 MaxItemsAmount = 5;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Throwables", meta = (UIMin = -90.0f, UIMax = 90.0f, ClampMin = -90.0f, ClampMax = 90.0f))
		float ThrowableAngle = 0.0f;

private:
	int32 Items;
};