// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraActor.h"
#include "InteractiveCameraActor.generated.h"

/**
 * 
 */
UCLASS()
class HOMEWORKPROJECT_API AInteractiveCameraActor : public ACameraActor
{
	GENERATED_BODY()
	
public:
	//конструктор где мы будем создавать компонент
	AInteractiveCameraActor();

protected:
	//будем подписывать функции нашего эктора на события нашего компонента
	virtual void BeginPlay() override;

	//компонент который будет пересекаться с pawn'ом
	UPROPERTY(VisibleAnywhere)
		class UBoxComponent* BoxComponent;

	//время за которое мы перейдем к камере
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Transition settings")
		float TransitionToCameraTime;

	//ИСП ЧТОБЫ ПЕРЕЙТИ К ВИДУ ИГРОКА
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Transition settings")
		float TransitionToPawnTime;

	//его мы подписываем на события нашего компонента
	UFUNCTION()
		void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
