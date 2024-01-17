// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "GameCodeBasePawn.generated.h"

UCLASS()
class HOMEWORKPROJECT_API AGameCodeBasePawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AGameCodeBasePawn();

	UPROPERTY(VisibleAnywhere)
		class UPawnMovementComponent* MovementComponent;

	UPROPERTY(VisibleAnywhere)
		class USphereComponent* CollisionComponent;

public:

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void MoveForward(float value);
	void MoveRight(float value);
	void Jump();

	UFUNCTION(BlueprintCallable, BlueprintPure)
		float GetInputForward() { return InputForward; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
		float GetInputRight() { return InputRight; }

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Base pawn")
		class USpringArmComponent* SpringArmComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Base pawn")
		float CollisionSphereRadius = 50.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Base pawn")
		class UCameraComponent* CameraComponent;

#if	WITH_EDITORONLY_DATA
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Base pawn")
		class UArrowComponent* ArrowComponent;
#endif

private:
	UFUNCTION()
		void OnBlendComplete();

	float InputForward = 0.0f;
	float InputRight = 0.0f;

	AActor* CurrentViewActor;
};
