// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GCBaseCharacter.h"
#include "Components/CharacterComponents/AIPatrollingComponent.h"
#include "GCAICharacter.generated.h"

/**
 * 
 */
class UBehaviorTree;

UCLASS(Blueprintable)
class HOMEWORKPROJECT_API AGCAICharacter : public AGCBaseCharacter
{
	GENERATED_BODY()
	
public:
	AGCAICharacter(const FObjectInitializer& ObjectInitializer);

	UAIPatrollingComponent* GetPatrollingComponent() const;

	UBehaviorTree* GetBahaviorTree() const;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		UAIPatrollingComponent* AIPatrollingComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI")
		UBehaviorTree* BehaviorTree;
};
