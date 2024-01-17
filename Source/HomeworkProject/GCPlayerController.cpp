// Fill out your copyright notice in the Description page of Project Settings.


#include "GCPlayerController.h"
#include "Engine/EngineBaseTypes.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CharacterComponents/CharacterEquipmentComponent.h"
#include "UI/Widget/AmmoWidget.h"
#include "GCBaseCharacterMovementComponent.h"
#include "GCBaseCharacter.h"
#include "UI/Widget/ReticleWidget.h"
#include "UI/Widget/PlayerHUDWidget.h"
#include "Blueprint/UserWidget.h"
#include "UI/Widget/WidgetCharacterAttributes.h"

void AGCPlayerController::SetPawn(APawn* InPawn)
{
	Super::SetPawn(InPawn);
	CachedBaseCharacter = Cast<AGCBaseCharacter>(InPawn);
	CreateAndInitializeWidgets();
}

bool AGCPlayerController::GetIgnoreCameraPitch() const
{
	return bIgnoreCameraPitch;
}

void AGCPlayerController::SetIgnoreCameraPitch(bool bIgnoreCameraPitch_In)
{
	bIgnoreCameraPitch = bIgnoreCameraPitch_In;
}

void AGCPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	InputComponent->BindAxis("MoveForward", this, &AGCPlayerController::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &AGCPlayerController::MoveRight);
	InputComponent->BindAxis("Turn", this, &AGCPlayerController::Turn);
	InputComponent->BindAxis("LookUp", this, &AGCPlayerController::LookUp);
	InputComponent->BindAxis("TurnAtRate", this, &AGCPlayerController::TurnAtRate);
	InputComponent->BindAxis("LookUpAtRate", this, &AGCPlayerController::LookUpAtRate);
	InputComponent->BindAxis("SwimForward", this, &AGCPlayerController::SwimForward);
	InputComponent->BindAxis("SwimRight", this, &AGCPlayerController::SwimRight);
	InputComponent->BindAxis("SwimUp", this, &AGCPlayerController::SwimUp);
	InputComponent->BindAxis("ClimbLadderUp", this, &AGCPlayerController::ClimbLadderUp);
	InputComponent->BindAction("Sliding", EInputEvent::IE_Pressed, this, &AGCPlayerController::Sliding);
	InputComponent->BindAction("DetachFromObject", EInputEvent::IE_Pressed, this, &AGCPlayerController::DetachFromWall);
	InputComponent->BindAction("InteractWithLadder", EInputEvent::IE_Pressed, this, &AGCPlayerController::InteractWithLadder);
	InputComponent->BindAction("InteractWithZipline", EInputEvent::IE_Pressed, this, &AGCPlayerController::InteractWithZipline);
	InputComponent->BindAction("Mantle", EInputEvent::IE_Pressed, this, &AGCPlayerController::Mantle);
	InputComponent->BindAction("Jump", EInputEvent::IE_Pressed, this, &AGCPlayerController::Jump);
	InputComponent->BindAction("Crouch", EInputEvent::IE_Pressed, this, &AGCPlayerController::ChangeCrouchState);
	InputComponent->BindAction("Prone", EInputEvent::IE_Pressed, this, &AGCPlayerController::ChangeProneState);
	InputComponent->BindAction("Sprint", EInputEvent::IE_Pressed, this, &AGCPlayerController::StartSprint);
	InputComponent->BindAction("Sprint", EInputEvent::IE_Released, this, &AGCPlayerController::StopSprint);
	InputComponent->BindAction("Fire", EInputEvent::IE_Pressed, this, &AGCPlayerController::PlayerStartFire);
	InputComponent->BindAction("Fire", EInputEvent::IE_Released, this, &AGCPlayerController::PlayerStopFire);
	InputComponent->BindAction("Aim", EInputEvent::IE_Pressed, this, &AGCPlayerController::StartAiming);
	InputComponent->BindAction("Aim", EInputEvent::IE_Released, this, &AGCPlayerController::StopAiming);
	InputComponent->BindAction("Reload", EInputEvent::IE_Pressed, this, &AGCPlayerController::Reload);
	InputComponent->BindAction("NextItem", EInputEvent::IE_Pressed, this, &AGCPlayerController::NextItem);
	InputComponent->BindAction("PreviousItem", EInputEvent::IE_Pressed, this, &AGCPlayerController::PreviousItem);
	InputComponent->BindAction("EquipPrimaryItem", EInputEvent::IE_Pressed, this, &AGCPlayerController::EquipPrimaryItem);
	InputComponent->BindAction("ChangeFireMode", EInputEvent::IE_Pressed, this, &AGCPlayerController::ChangeFireMode);
	InputComponent->BindAction("PrimaryMeleeAttack", EInputEvent::IE_Pressed, this, &AGCPlayerController::PrimaryMeleeAttack);
	InputComponent->BindAction("SecondaryMeleeAttack", EInputEvent::IE_Pressed, this, &AGCPlayerController::SecondaryMeleeAttack);
}

void AGCPlayerController::MoveForward(float value)
{
	if (CachedBaseCharacter.IsValid())
	{
		CachedBaseCharacter->MoveForward(value);
	}
}

void AGCPlayerController::MoveRight(float value)
{
	if (CachedBaseCharacter.IsValid())
	{
		CachedBaseCharacter->MoveRight(value);
	}
}

void AGCPlayerController::Turn(float value)
{
	if (CachedBaseCharacter.IsValid())
	{
		CachedBaseCharacter->Turn(value);
	}
}

void AGCPlayerController::LookUp(float value)
{
	if (CachedBaseCharacter.IsValid())
	{
		CachedBaseCharacter->LookUp(value);
	}
}

void AGCPlayerController::TurnAtRate(float value)
{
	if (CachedBaseCharacter.IsValid())
	{
		CachedBaseCharacter->TurnAtRate(value);
	}
}

void AGCPlayerController::LookUpAtRate(float value)
{
	if (CachedBaseCharacter.IsValid())
	{
		CachedBaseCharacter->LookUpAtRate(value);
	}
}

void AGCPlayerController::ChangeCrouchState()
{
	if (CachedBaseCharacter.IsValid())
	{
		CachedBaseCharacter->ChangeCrouchState();
	}
}

void AGCPlayerController::Jump()
{
	if (CachedBaseCharacter.IsValid())
	{
		CachedBaseCharacter->TryToJump();
	}
}

void AGCPlayerController::ChangeProneState()
{
	if (CachedBaseCharacter.IsValid())
	{
		CachedBaseCharacter->ChangeProneState();
	}
}

void AGCPlayerController::Mantle()
{
	if (CachedBaseCharacter.IsValid())
	{
		CachedBaseCharacter->Mantle();
	}
}

void AGCPlayerController::StartSprint()
{
	if (CachedBaseCharacter.IsValid())
	{
		CachedBaseCharacter->StartSprint();
	}
}

void AGCPlayerController::StopSprint()
{
	if (CachedBaseCharacter.IsValid())
	{
		CachedBaseCharacter->StopSprint();
	}
}

void AGCPlayerController::SwimForward(float Value)
{
	if (CachedBaseCharacter.IsValid())
	{
		CachedBaseCharacter->SwimForward(Value);
	}
}

void AGCPlayerController::SwimRight(float Value)
{
	if (CachedBaseCharacter.IsValid())
	{
		CachedBaseCharacter->SwimRight(Value);
	}
}

void AGCPlayerController::SwimUp(float Value)
{
	if (CachedBaseCharacter.IsValid())
	{
		CachedBaseCharacter->SwimUp(Value);
	}
}

void AGCPlayerController::ClimbLadderUp(float Value)
{
	if (CachedBaseCharacter.IsValid())
	{
		CachedBaseCharacter->ClimbLadderUp(Value);
	}
}

void AGCPlayerController::InteractWithLadder()
{
	if (CachedBaseCharacter.IsValid())
	{
		CachedBaseCharacter->InteractWithLadder();
	}
}

void AGCPlayerController::InteractWithZipline()
{
	if (CachedBaseCharacter.IsValid())
	{
		CachedBaseCharacter->InteractWithZipline();
	}
}

void AGCPlayerController::DetachFromWall()
{
	if (CachedBaseCharacter.IsValid())
	{
		CachedBaseCharacter->GetBaseCharacterMovementComponent()->JumpFromWall();
	}
}

void AGCPlayerController::Sliding()
{
	if (CachedBaseCharacter.IsValid())
	{
		CachedBaseCharacter->Sliding();
	}
}

void AGCPlayerController::PlayerStopFire()
{
	if (CachedBaseCharacter.IsValid())
	{
		CachedBaseCharacter->StopFire();
	}
}

void AGCPlayerController::PlayerStartFire()
{
	if (CachedBaseCharacter.IsValid())
	{
		CachedBaseCharacter->StartFire();
	}
}

void AGCPlayerController::StartAiming()
{
	if (CachedBaseCharacter.IsValid())
	{
		CachedBaseCharacter->StartAiming();
	}
}

void AGCPlayerController::StopAiming()
{
	if (CachedBaseCharacter.IsValid())
	{
		CachedBaseCharacter->StopAiming();
	}
}

void AGCPlayerController::NextItem()
{
	if (CachedBaseCharacter.IsValid())
	{
		CachedBaseCharacter->NextItem();
	}
}

void AGCPlayerController::PreviousItem()
{
	if (CachedBaseCharacter.IsValid())
	{
		CachedBaseCharacter->PreviousItem();
	}
}

void AGCPlayerController::EquipPrimaryItem()
{
	if (CachedBaseCharacter.IsValid())
	{
		CachedBaseCharacter->EquipPrimaryItem();
	}
}

void AGCPlayerController::Reload()
{
	if (CachedBaseCharacter.IsValid())
	{
		CachedBaseCharacter->Reload();
	}
}

void AGCPlayerController::ChangeFireMode()
{
	if (CachedBaseCharacter.IsValid())
	{
		CachedBaseCharacter->ChangeFireMode();
	}
}

void AGCPlayerController::PrimaryMeleeAttack()
{
	if (CachedBaseCharacter.IsValid())
	{
		CachedBaseCharacter->PrimaryMeleeAttack();
	}
}

void AGCPlayerController::SecondaryMeleeAttack()
{
	if (CachedBaseCharacter.IsValid())
	{
		CachedBaseCharacter->SecondaryMeleeAttack();
	}
}

void AGCPlayerController::CreateAndInitializeWidgets()
{
	if (!IsValid(PlayerHUDWidget))
	{
		PlayerHUDWidget = CreateWidget<UPlayerHUDWidget>(GetWorld(), PlayerHUDWidgetClass);
		if (IsValid(PlayerHUDWidget))
		{
			PlayerHUDWidget->AddToViewport();
		}
	}

	if (IsValid(PlayerHUDWidget) && CachedBaseCharacter.IsValid())
	{
		UReticleWidget* ReticleWidget = PlayerHUDWidget->GetReticleWidget();
		if (IsValid(ReticleWidget))
		{
			CachedBaseCharacter->OnAimingStateChanged.AddUFunction(ReticleWidget, FName("OnAimingStateChanged"));
			UCharacterEquipmentComponent* CharacterEquipment = CachedBaseCharacter->GetCharacterEquipmentComponent_Mutable();
			CharacterEquipment->OnEquippedItemChanged.AddUFunction(ReticleWidget, FName("OnEquippedItemChanged"));
		}

		UAmmoWidget* AmmoWidget = PlayerHUDWidget->GetAmmoWidget();
		if (IsValid(AmmoWidget))
		{
			UCharacterEquipmentComponent* CharacterEquipment = CachedBaseCharacter->GetCharacterEquipmentComponent_Mutable();
			CharacterEquipment->OnCurrentWeaponAmmoChangedEvent.AddUFunction(AmmoWidget, FName("UpdateAmmoCount"));
			CharacterEquipment->OnCurrentThrowableItemsAmountChangedEvent.AddUFunction(AmmoWidget, FName("UpdateThrowableItems"));
		}

		UWidgetCharacterAttributes* HealthBar = PlayerHUDWidget->GetWidgetCharacterAttributes(EAttributesType::Health);
		if (IsValid(HealthBar))
		{
			CachedBaseCharacter->HealthPercentEvent.AddUObject(HealthBar, &UWidgetCharacterAttributes::UpdateHealthPercent);
		}
		
		UWidgetCharacterAttributes* OxygenBar = PlayerHUDWidget->GetWidgetCharacterAttributes(EAttributesType::Oxygen);
		if (IsValid(OxygenBar))
		{
			CachedBaseCharacter->OxygenPercentEvent.AddUObject(OxygenBar, &UWidgetCharacterAttributes::UpdateOxygenPercent);
		}
		
		UWidgetCharacterAttributes* StaminaBar = PlayerHUDWidget->GetWidgetCharacterAttributes(EAttributesType::Stamina);
		if (IsValid(StaminaBar))
		{
			CachedBaseCharacter->StaminaPercentEvent.AddUObject(StaminaBar, &UWidgetCharacterAttributes::UpdateStaminaPercent);
		}
	}
}