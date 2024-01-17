// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GCBaseCharacterMovementComponent.h"
#include "CharacterAttributesComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnDeathEventSignature);
DECLARE_MULTICAST_DELEGATE_OneParam(FOutOfStaminaEventSignature, bool);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class HOMEWORKPROJECT_API UCharacterAttributesComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCharacterAttributesComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	FORCEINLINE UGCBaseCharacterMovementComponent* GetBaseCharacterMovementComponent() const { return GCBaseCharacterMovementComponent; }

	FOnDeathEventSignature OnDeathEvent;

	FOutOfStaminaEventSignature OutOfStamina;

	bool IsAlive() { return Health > 0.0f; }

	float GetHealthPercent() const;

	float GetOxygenPercent() const;

	float GetStaminaPercent() const;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
		
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Health", meta = (UIMin = 0.0f))
		float MaxHealth = 100.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stamina", meta = (ClampMin = 0.0f, UIMin = 0.0f))
		float MaxStamina; // ������������ �������� ������������ ������

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stamina", meta = (ClampMin = 0.0f, UIMin = 0.0f))
		float StaminaRestoreVelocity; // �������� �������������� ������������

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stamina", meta = (ClampMin = 0.0f, UIMin = 0.0f))
		float SprintStaminaConsumptionVelocity; // �������� ����������� ������������ ������

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Oxygen", meta = (ClampMin = 0.0f, UIMin = 0.0f))
		float MaxOxygen = 50.0f; // ������������ ����� ���������

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Oxygen", meta = (ClampMin = 0.0f, UIMin = 0.0f))
		float OxygenRestoreVelocity = 15.0f; // �������� �������������� ��������� � �������

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Oxygen", meta = (ClampMin = 0.0f, UIMin = 0.0f))
		float SwimOxygenConsumptionVelocity = 2.0f; // �������� ����������� ��������� �� ����� �������� ��� ����� � �������

private:
	float Health = 0.0f;
	float CurrentStamina;
	float Oxygen;
	float CurrentOxygen;

	UGCBaseCharacterMovementComponent* GCBaseCharacterMovementComponent;

#if UE_BUILD_DEBUG || UE_BUILD_DEVELOPMENT
	void DebugDrawAttributes();//��������� �������� 
#endif

	UFUNCTION()
		void OnTakeAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

	void UpdateStaminaValue(float DeltaTime);
	void UpdateOxygenValue(float DeltaTime);
	bool IsSwimmingUnderWater(float HeadLocation, float VolumeLocation) const;

	TWeakObjectPtr<class AGCBaseCharacter> CachedBaseCharacterOwner;
};
