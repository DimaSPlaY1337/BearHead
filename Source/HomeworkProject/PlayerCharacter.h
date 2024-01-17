// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GCBaseCharacter.h"
#include "PlayerCharacter.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class HOMEWORKPROJECT_API APlayerCharacter : public AGCBaseCharacter
{
	GENERATED_BODY()

public:
    APlayerCharacter(const FObjectInitializer& ObjectInitializer);

    virtual void BeginPlay() override;

    virtual void MoveForward(float Value) override;
    virtual void MoveRight(float Value) override;
    virtual void Turn(float Value) override;
    virtual void LookUp(float Value) override;
    virtual void TurnAtRate(float Value) override;
    virtual void LookUpAtRate(float Value) override;

    virtual void OnStartCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust) override;
    virtual void OnEndCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust) override;

    virtual bool CanJumpInternal_Implementation() const override;
    virtual void OnJumped_Implementation() override;

    virtual void OnStartProne(float HalfHeightAdjust);
    virtual void OnEndProne(float HalfHeightAdjust);

    virtual void OnStartSlide(float HalfHeightAdjust);
    virtual void OnEndSlide(float HalfHeightAdjust);

    virtual void SwimForward(float Value) override;
    virtual void SwimRight(float Value) override;
    virtual void SwimUp(float Value) override;

    virtual void Tick(float DeltaTime) override;
    float GetForwardAxis() { return ForwardAxis; }
    float GetRightAxis() { return RightAxis; }

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character | Camera")
        class UCameraComponent* CameraComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character | Camera")
        class USpringArmComponent* SpringArmComponent;

    void AimCameraUpdate(float Value);

protected:
    virtual void OnStartAimingInternal() override;
    virtual void OnStopAimingInternal() override;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character | AIM")
        UCurveFloat* AimCurve;

    void SetIsAiming(float Value);

private:
    bool IsAiming;
    FTimeline AimingTimeline;
    float ForwardAxis;
    float RightAxis;
};
