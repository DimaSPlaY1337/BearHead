// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameCodeTypes.h"
#include "ReticleWidget.generated.h"

/**
 * 
 */
class AEquipableItem;
UCLASS()
class HOMEWORKPROJECT_API UReticleWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	//событие которое вызывается когда будем прицеливаться
	UFUNCTION(BlueprintNativeEvent)
	void OnAimingStateChanged(bool bIsAiming);

	UFUNCTION(BlueprintNativeEvent)
		void OnEquippedItemChanged(const AEquipableItem* EquippedItem);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Reticle")
		EReticleType CurrentReticle = EReticleType::None;

private:
	TWeakObjectPtr<const AEquipableItem> CurrentEquippedItem;

	void SetupCurrentReticle();
};
