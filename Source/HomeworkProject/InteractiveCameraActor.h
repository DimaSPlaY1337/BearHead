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
	//����������� ��� �� ����� ��������� ���������
	AInteractiveCameraActor();

protected:
	//����� ����������� ������� ������ ������ �� ������� ������ ����������
	virtual void BeginPlay() override;

	//��������� ������� ����� ������������ � pawn'��
	UPROPERTY(VisibleAnywhere)
		class UBoxComponent* BoxComponent;

	//����� �� ������� �� �������� � ������
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Transition settings")
		float TransitionToCameraTime;

	//��� ����� ������� � ���� ������
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Transition settings")
		float TransitionToPawnTime;

	//��� �� ����������� �� ������� ������ ����������
	UFUNCTION()
		void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
