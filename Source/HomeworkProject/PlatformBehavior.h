// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlatformBehavior.generated.h"

UENUM()
enum class EPlatformBehavior : uint8
{
	OnDemand = 0, /*-платформа перемещается только при запросе*/
	Loop //- платформа перемещается по циклу.
};
