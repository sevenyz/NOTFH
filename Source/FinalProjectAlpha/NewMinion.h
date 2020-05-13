// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BlackboardData.h"
#include "AIController.h"

#include "NewMinion.generated.h"

UCLASS()
class FINALPROJECTALPHA_API ANewMinion : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ANewMinion();

	UPROPERTY(BlueprintReadOnly)
	float StunTime;

	UPROPERTY(EditAnywhere)
	int HP = 100;

	UPROPERTY(EditAnywhere, Category = "Params Minion")
	int Damage = 5;

	UPROPERTY()
	bool bCanAttack = true;

	UPROPERTY(EditAnywhere, Category = "Magic Essence")
	TSubclassOf<class AMagical_Essence> MagicalEssence;

	class AFinalProjectAlphaCharacter* PlayerRef;

	FTimerHandle TimerAttack;

	FTimerHandle TimerControlBoolStun;

	FTimerHandle TrapTimerHandle;

	UPROPERTY(EditAnywhere)
	UBlackboardData* BlackboardToUse;

	UBlackboardComponent* Blackboard;

	FName IsStunnedKeyName = "IsStunned";
	FName IsHittedKeyName = "IsHitted";



	UFUNCTION(BlueprintCallable)
	void Attack();

	UFUNCTION()
	void ChangeSpeed(float Speed, float Duration);

	UFUNCTION()
	void BlockRotation();

	UFUNCTION()
	void SpeedReset();

	UFUNCTION(BlueprintCallable)
	void CalculateDamage(int damageDirect);

	UFUNCTION(BlueprintImplementableEvent)
	void Death();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
