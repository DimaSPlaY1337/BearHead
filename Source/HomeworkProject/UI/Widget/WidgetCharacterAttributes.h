// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WidgetCharacterAttributes.generated.h"

/**
 * 
 */
UCLASS()
class HOMEWORKPROJECT_API UWidgetCharacterAttributes : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "Health")
	float HealthPercent = 1.0f;
	
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "Oxygen")
	float OxygenPercent = 1.0f;
	
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "Stamina")
	float StaminaPercent = 1.0f;

	const UCharacterAttributesComponent* GetAttributesComponent();

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "Stamina")
		ESlateVisibility OxygenVisibilityStatus = ESlateVisibility::Hidden;
	
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "Stamina")
		ESlateVisibility StaminaVisibilityStatus = ESlateVisibility::Hidden;

	/*UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		ESlateVisibility GetNewVisibilityStatus();*/
public:
	void UpdateHealthPercent();
	void UpdateOxygenPercent(ESlateVisibility Vis);
	void UpdateStaminaPercent(ESlateVisibility Vis);

	void SetVisibilityStatus(ESlateVisibility Vis, EAttributesType Type);
};
