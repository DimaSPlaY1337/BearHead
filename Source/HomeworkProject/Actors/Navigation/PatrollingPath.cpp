// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Navigation/PatrollingPath.h"

const TArray<FVector>& APatrollingPath::GetWayPoints() const
{
	return WayPoints;
}