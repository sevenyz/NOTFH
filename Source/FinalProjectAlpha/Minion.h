// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"

#include "Components/SkeletalMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Components/ArrowComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BlackboardData.h"
#include "AIController.h"

#include "Minion.generated.h"

UCLASS()
class FINALPROJECTALPHA_API AMinion : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AMinion();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USkeletalMeshComponent* MyMesh;

	UPROPERTY(VisibleAnywhere)
	UBoxComponent* MyBox;

	UPROPERTY(VisibleAnywhere)
	UArrowComponent* ArrowComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UFloatingPawnMovement* PawnMovement;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UBoxComponent* AttackCollider;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float StunTime;

	UPROPERTY(BlueprintReadOnly)
	bool bCanAttack;

	UPROPERTY(EditAnywhere)
	int HP = 100;

	class AFinalProjectAlphaCharacter* PlayerRef;

	FTimerHandle TimerAttack;

	FTimerHandle TimerControlBoolStun;

	FTimerHandle TrapTimerHandle;

	UPROPERTY(EditAnywhere)
	UBlackboardData* BlackboardToUse;

	UBlackboardComponent* Blackboard;

	FName IsStunnedKeyName = "IsStunned";

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void AttackOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void AttackOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION(BlueprintCallable)
	void Attack();

	UFUNCTION()
	void ChangeSpeed(float Speed, float Duration);

	UFUNCTION()
	void BlockRotation();

	UFUNCTION()
	void SpeedReset();

	UFUNCTION()
	void TakeDamage(int Damage);
};
