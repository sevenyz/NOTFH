// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BlackboardData.h"
#include "AIController.h"
#include "Boss.generated.h"

UCLASS()
class FINALPROJECTALPHA_API ABoss : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABoss();

	UPROPERTY()
	FTimerHandle TrapTimerHandle;

	UPROPERTY(EditAnywhere)
	UBlackboardData* BlackboardToUse;

	UPROPERTY(EditAnywhere)
	UBlackboardComponent* Blackboard;

	UPROPERTY(BlueprintReadWrite)
	FName IsStunnedKeyName = "IsStunned";

	UPROPERTY()
	FName IsHitKeyName = "IsHit";
	
	UPROPERTY()
	FName HasLineOfSightKeyName = "HasLineOfSight";

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bIsStunned;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Parameters")
	float MaxHP = 100;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Parameters")
	float CurrentHP = MaxHP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Parameters")
	int	AttackDamage = 20;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Parameters")
	float HeatShockMultiplier = 1;

	UPROPERTY(BlueprintReadWrite, Category = "Parameters")
	float HeatShockDamage = 1;

	UPROPERTY(VisibleAnywhere, Category = "Parameters")
	bool bBerserkMood = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Parameters")
	bool bBossStun = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Parameters")
	bool bIsBombDamage = false;

	UPROPERTY(VisibleAnywhere)
	float TimerCheck = 4.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Parameters")
	float StunTime;

	UPROPERTY(EditAnywhere, Category = "Berserk Mood")
	float TimerDurationBerserkMood = 10.0f;

	UPROPERTY(EditAnywhere, Category = "Berserk Mood")
	float VelocityMovementBerserkMood = 600.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Parameters")
	float Phase2 = 0.75f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Parameters")
	float Phase3 = 0.40f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Parameters")
	float NormalAttackCooldown = 0.3f;

	FTimerHandle TimerControlBoolStun;

	FTimerHandle TimerControlBoolDamage;

	FTimerHandle TimerBerserkMood;

	FTimerHandle TimerAttack;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UBoxComponent* AttackCollider;

	UPROPERTY(BlueprintReadOnly)
	bool bCanAttack = true;

	UPROPERTY(BlueprintReadOnly)
	bool bPlayerInArea = false;

	UPROPERTY(BlueprintReadOnly)
	class AFinalProjectAlphaCharacter* PlayerRef;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void ChangeSpeed(float Speed, float Duration);

	UFUNCTION(BlueprintCallable)
	void BlockRotation();

	UFUNCTION()
	void SpeedReset();

	UFUNCTION(BlueprintCallable)
	void CalculateDamage(int NormalDamage);

	UFUNCTION()
	void Berserk();

	UFUNCTION()
	void ControlBoolTrapBoss();

	UFUNCTION()
	void ResetBoolTrapBoss();

	UFUNCTION()
	void AttackOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void AttackOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION(BlueprintCallable)
	void Attack();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void Death();
};
