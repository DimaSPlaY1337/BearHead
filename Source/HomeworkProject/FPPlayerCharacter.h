// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerCharacter.h"
#include "FPPlayerCharacter.generated.h"

/**
 * 
 */
UCLASS()
class HOMEWORKPROJECT_API AFPPlayerCharacter : public APlayerCharacter
{
	GENERATED_BODY()

public:
	AFPPlayerCharacter(const FObjectInitializer& ObjectInitializer);

	virtual void PossessedBy(AController* NewController) override;//в тот момент когда какой-то контроллер начинает обладать персонажем, вызывается

	virtual void Tick(float DeltaTime) override;

	virtual void OnStartCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust) override;

	virtual void OnEndCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust) override;

	virtual FRotator GetViewRotation() const override;//возвращает ротатор камеры(в направлении куда смотрит)

	virtual void OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PreviousCustomMode = 0) override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character | First person")
		class USkeletalMeshComponent* FirstPersonMeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character | First person")
		class UCameraComponent* FirstPersonCameraComponent;

	virtual void OnMantle(const FMantlingSettings& MantlingSettings, float MantlingAnimationStartTime) override;

	virtual void Falling() override;

	virtual void OnHardLanding();

	virtual void Landed(const FHitResult& Hit) override;

	virtual void NotifyJumpApex() override;

	UPROPERTY(EditDefaultsOnly , BlueprintReadOnly, Category = "Character | First person | Camera | Ladder | Pitch", meta = (UIMin = -89.0f, UIMax = 89.0f))
		float LadderCameraMinPitch = -60.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character | First person | Camera | Ladder | Pitch", meta = (UIMin = -89.0f, UIMax = 89.0f))
		float LadderCameraMaxPitch = 80.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character | First person | Camera | Ladder | Yaw", meta = (UIMin = 0.0f, UIMax = 359.0f))
		float LadderCameraMinYaw = 60.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character | First person | Camera | Ladder | Yaw", meta = (UIMin = -89.0f, UIMax = 89.0f))
		float LadderCameraMaxYaw = 120.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character | First person | Camera | Zipline | Pitch", meta = (UIMin = -89.0f, UIMax = 89.0f))
		float ZiplineCameraMinPitch = -89.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character | First person | Camera | Zipline | Pitch", meta = (UIMin = -89.0f, UIMax = 89.0f))
		float ZiplineCameraMaxPitch = 89.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character | First person | Camera | Zipline | Yaw", meta = (UIMin = 0.0f, UIMax = 359.0f))
		float ZiplineCameraMinYaw = 0.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character | First person | Camera | Zipline | Yaw", meta = (UIMin = -89.0f, UIMax = 89.0f))
		float ZiplineCameraMaxYaw = 180.0f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character | First person | Camera | Hard landing", meta = (UIMin = -89.0f, UIMax = 89.0f))
		float HardLandingMaxHeightDelta = 1000.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character | First person | Camera | Hard landing", meta = (UIMin = -89.0f, UIMax = 89.0f))
		class UAnimMontage* FPHardLandingMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character | First person | Camera | Hard landing", meta = (UIMin = -89.0f, UIMax = 89.0f))
		class UAnimMontage* TPHardLandingMontage;

private:
	float TheHighestJumpPoint;
	FTimerHandle FPMontageTimer;

	bool IsFPMontagePlaying() const;
	void OnFPMontageTimerElapsed();
	void OnLadderStarted();
	void OnLadderStopped();

	void OnZiplineStarted();
	void OnZiplineStopped();

	TWeakObjectPtr<class AGCPlayerController> GCPlayerController;
};
