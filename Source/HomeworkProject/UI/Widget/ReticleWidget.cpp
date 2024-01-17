// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/ReticleWidget.h"
#include "Actors/Equipment/EquipableItem.h"

void UReticleWidget::OnEquippedItemChanged_Implementation(const AEquipableItem* EquippedItem)
{
	CurrentEquippedItem = EquippedItem;
	SetupCurrentReticle();
}

void UReticleWidget::OnAimingStateChanged_Implementation(bool bIsAiming)
{
	SetupCurrentReticle();
}

void UReticleWidget::SetupCurrentReticle()
{
	CurrentReticle = CurrentEquippedItem.IsValid() ? CurrentEquippedItem->GetReticleType() : EReticleType::None;
}
