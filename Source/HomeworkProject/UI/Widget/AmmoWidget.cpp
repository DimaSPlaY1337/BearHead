// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/AmmoWidget.h"
#include "AmmoWidget.h"

void UAmmoWidget::UpdateAmmoCount(int32 NewAmmo, int32 NewTotalAmmo)
{
	Ammo = NewAmmo;
	TotalAmmo = NewTotalAmmo;
}

void UAmmoWidget::UpdateThrowableItems(int32 NewItems)
{
	Items = NewItems;
}
