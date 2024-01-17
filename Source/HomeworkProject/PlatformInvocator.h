// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PlatformInvocator.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInvocatorActivated);

UCLASS()
class HOMEWORKPROJECT_API APlatformInvocator : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	APlatformInvocator();

public:

	UPROPERTY(BlueprintAssignable)
	FOnInvocatorActivated OnInvocatorActivated;

	UFUNCTION(BlueprintCallable, Category = "MyFunc")
	void Invoke();
};
