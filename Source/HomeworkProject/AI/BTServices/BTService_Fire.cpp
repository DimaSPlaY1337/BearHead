// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTServices/BTService_Fire.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GCBaseCharacter.h"
#include "Actors/Equipment/Weapons/RangeWeaponItem.h"
#include "AIController.h"
#include "Components/CharacterComponents/CharacterEquipmentComponent.h"

UBTService_Fire::UBTService_Fire()
{
	NodeName = "Fire";
}

void UBTService_Fire::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	AAIController* AIController = OwnerComp.GetAIOwner();
	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();

	if (!IsValid(AIController) || !IsValid(Blackboard))
	{
		return;
	}

	AGCBaseCharacter* Character = Cast<AGCBaseCharacter>(AIController->GetPawn());
	if (!IsValid(Character))
	{
		return;
	}

	const UCharacterEquipmentComponent* EquipmentComponent = Character->GetCharacterEquipmentComponent();
	ARangeWeaponItem* RangeWeapon = EquipmentComponent->GetCurrentRangeWeapon();

	if (!IsValid(RangeWeapon))
	{
		return;
	}

	AActor* CurrentTraget = Cast<AActor>(Blackboard->GetValueAsObject(TargetKey.SelectedKeyName));
	if (!IsValid(CurrentTraget))
	{
		Character->StopFire();
		return;
	}

	float DistSq = FVector::DistSquared(CurrentTraget->GetActorLocation(), Character->GetActorLocation());
	if (DistSq > FMath::Square(MaxFireDistance))
	{
		Character->StopFire();
		return;
	}

	if (!(RangeWeapon->IsReloading() || RangeWeapon->IsFiring()))
	{
		Character->StartFire();
	}
}