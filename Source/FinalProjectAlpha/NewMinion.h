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

	UPROPERTY(BlueprintReadWrite)
	float StunTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Parameters")
	float MaxHP = 100;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Parameters")
	float CurrentHP = MaxHP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Parameters")
	int Damage = 5;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bCanAttack = true;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UBoxComponent* AttackCollider;

	UPROPERTY(EditAnywhere, Category = "Magic Essence")
	TSubclassOf<class AMagical_Essence> MagicalEssence;

	UPROPERTY(BlueprintReadWrite)
	class AFinalProjectAlphaCharacter* PlayerRef;

	FTimerHandle TimerAttack;

	FTimerHandle TimerControlBoolStun;

	FTimerHandle TrapTimerHandle;

	UPROPERTY(EditAnywhere)
	UBlackboardData* BlackboardToUse;

	UBlackboardComponent* Blackboard;

	FName IsStunnedKeyName = "IsStunned";
	FName IsHitKeyName = "IsHit";

	UFUNCTION(BlueprintCallable)
	void Attack();

	UFUNCTION(BlueprintCallable)
	void ChangeSpeed(float Speed, float Duration);

	UFUNCTION(BlueprintCallable)
	void BlockRotation();

	UFUNCTION()
	void SpeedReset();

	UFUNCTION(BlueprintCallable)
	void CalculateDamage(int damageDirect);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void Death();

	UFUNCTION()
	void AttackOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void AttackOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
