// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ReticleWidget.h"
#include "PlayerHUDWidget.generated.h"

/**
 * 
 */
UCLASS()
class HOMEWORKPROJECT_API UPlayerHUDWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	class UReticleWidget* GetReticleWidget();

	class UAmmoWidget* GetAmmoWidget();

	class UWidgetCharacterAttributes* GetWidgetCharacterAttributes(EAttributesType Type);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Widget names")
		FName ReticleWidgetName;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Widget names")
		FName AmmoWidgetName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Widget names")
		FName HealthWidgetName;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Widget names")
		FName OxygenWidgetName;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Widget names")
		FName StaminaWidgetName;
};
