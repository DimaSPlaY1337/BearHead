// Fill out your copyright notice in the Description page of Project Settings.


#include "UAnimNotify_EnableRagdoll.h"
#include "GameCodeTypes.h"

void UUAnimNotify_EnableRagdoll::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	MeshComp->SetCollisionProfileName(CollisionProfileRagdoll);
	MeshComp->SetSimulatePhysics(true);
}