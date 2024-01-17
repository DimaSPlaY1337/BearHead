// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Characters/GCAICharacter.h"

AGCAICharacter::AGCAICharacter(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	AIPatrollingComponent = CreateDefaultSubobject<UAIPatrollingComponent>(TEXT("AIPatrolling"));

}

UAIPatrollingComponent* AGCAICharacter::GetPatrollingComponent() const
{
	return AIPatrollingComponent;
}

UBehaviorTree* AGCAICharacter::GetBahaviorTree() const
{
	return BehaviorTree;
}
