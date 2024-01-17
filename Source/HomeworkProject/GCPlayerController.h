// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "UObject/SoftObjectPtr.h"
#include "GCBaseCharacterMovementComponent.h"
#include "GCPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class HOMEWORKPROJECT_API AGCPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	virtual void SetPawn(APawn* InPawn) override;
	
	bool GetIgnoreCameraPitch() const;

	void SetIgnoreCameraPitch(bool bIgnoreCameraPitch_In);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Widgets")
		TSubclassOf<class UPlayerHUDWidget> PlayerHUDWidgetClass;

	virtual void SetupInputComponent() override;

private:
	void MoveForward(float value);
	void MoveRight(float value);
	void Turn(float value);
	void LookUp(float value);
	void TurnAtRate(float value);
	void LookUpAtRate(float value);
	void ChangeCrouchState();
	void Jump();
	void ChangeProneState();
	void Mantle();

	void StartSprint();
	void StopSprint();

	void SwimForward(float Value);
	void SwimRight(float Value);
	void SwimUp(float Value);

	void ClimbLadderUp(float Value);
	void InteractWithLadder();

	void InteractWithZipline();
	void DetachFromWall();
	void Sliding();

	void PlayerStartFire();
	void PlayerStopFire();

	void StartAiming();
	void StopAiming();

	void NextItem();
	void PreviousItem();

	void EquipPrimaryItem();
	void Reload();

	void CreateAndInitializeWidgets();

	void ChangeFireMode();

	void PrimaryMeleeAttack();
	void SecondaryMeleeAttack();

	TSoftObjectPtr<class AGCBaseCharacter> CachedBaseCharacter;
	
	UPlayerHUDWidget* PlayerHUDWidget = nullptr;

	bool bIgnoreCameraPitch = false;
};
