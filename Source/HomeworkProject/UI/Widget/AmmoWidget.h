// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AmmoWidget.generated.h"

/**
 * 
 */
UCLASS()
class HOMEWORKPROJECT_API UAmmoWidget : public UUserWidget
{
	GENERATED_BODY()

protected:

	//текущий боезапас
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "Ammo")
		int32 Ammo;
	
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "Ammo")
		int32 TotalAmmo;
	
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "ThrowableItem")
		int32 Items;

private:
	UFUNCTION()
	void UpdateAmmoCount(int32 NewAmmo, int32 NewTotalAmmo);

	UFUNCTION()
	void UpdateThrowableItems(int32 NewItems);
};
