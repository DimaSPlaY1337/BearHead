// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/WidgetCharacterAttributes.h"
#include "GCBaseCharacter.h"
#include "CharacterAttributesComponent.h"
#include "WidgetCharacterAttributes.h"

void UWidgetCharacterAttributes::UpdateHealthPercent()
{
	if (IsValid(GetAttributesComponent()))
	{
		HealthPercent = GetAttributesComponent()->GetHealthPercent();
	}
}

const UCharacterAttributesComponent* UWidgetCharacterAttributes::GetAttributesComponent()
{
	APawn* Pawn = GetOwningPlayerPawn();
	AGCBaseCharacter* Character = Cast<AGCBaseCharacter>(Pawn);
	if (IsValid(Character))
	{
		const UCharacterAttributesComponent* CharacterAttributes = Character->GetCharacterAttributesComponent();
		return CharacterAttributes;
	}
	else
	{
		return nullptr;
	}
}

void UWidgetCharacterAttributes::UpdateOxygenPercent(ESlateVisibility Vis)
{
	SetVisibilityStatus(Vis, EAttributesType::Oxygen);
	if (IsValid(GetAttributesComponent()))
	{
		OxygenPercent = GetAttributesComponent()->GetOxygenPercent();
	}
}

void UWidgetCharacterAttributes::UpdateStaminaPercent(ESlateVisibility Vis)
{
	SetVisibilityStatus(Vis, EAttributesType::Stamina);
	if (IsValid(GetAttributesComponent()))
	{
		StaminaPercent = GetAttributesComponent()->GetStaminaPercent();
	}
}

void UWidgetCharacterAttributes::SetVisibilityStatus(ESlateVisibility Vis, EAttributesType Type)
{
	switch (Type)
	{
	case EAttributesType::Oxygen:
	{
		OxygenVisibilityStatus = Vis;
	}
	case EAttributesType::Stamina:
	{
		StaminaVisibilityStatus = Vis;
	}
	default:
		break;
	}
}