// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Player/BackpackPL.h"
#include "Utility/DelegateMaster.h"
#include "FinalProjectAlphaCharacter.generated.h"

UCLASS(config=Game)
class AFinalProjectAlphaCharacter : public ACharacter
{
	GENERATED_BODY()

#pragma region Camera Components

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

#pragma endregion


public:

	AFinalProjectAlphaCharacter();

	virtual void Tick(float DeltaTime) override;

#pragma region LineTrace Components

	UPROPERTY(VisibleAnywhere, Category = "LineTrace")
	USceneComponent* LineTraceStartPoint;

	UPROPERTY(VisibleAnywhere, Category = "LineTrace")
	USceneComponent* LineTraceEndPoint;

#pragma endregion

#pragma region Attack Components

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UBoxComponent* AttackCollider;

#pragma endregion

#pragma region Trap Parameters

	UPROPERTY()
	FTimerHandle TrapTimerHandle;

	UPROPERTY()
	bool CraftingMood;

#pragma endregion

#pragma region Player Parameters

	UPROPERTY(EditAnywhere, Category = "Parameters")
	int MaxHP = 100;

	UPROPERTY(EditAnywhere, Category = "Parameters")
	int Damage = 20;

	UPROPERTY(EditAnywhere, Category = "Parameters")
	float MaxSprint = 700.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Parameters")
	float NormalSpeed = 600.0f;

	UPROPERTY(EditAnywhere, Category = "Parameters")
	float MaxRotation = 540.0f;

	UPROPERTY(EditAnywhere, Category = "Parameters Attack")
	bool bBossInArea = false;

	UPROPERTY(EditAnywhere, Category = "Parameters Attack")
	bool bMinionArea = false;

	UPROPERTY(EditAnywhere, Category = "Parameters Attack")
	bool bCanAttack = true;

	UPROPERTY(VisibleAnywhere, Category = "Reference")
	class ABoss* BossRef;

	UPROPERTY(VisibleAnywhere, Category = "Reference")
	class AMinion* minionRef;

	UPROPERTY(VisibleAnywhere, Category = "Reference")
	class ADelegateMaster* delegateMaster;

	UPROPERTY(EditAnywhere, Category = "Parameters Attack")
	class AMyPlayerController* PlayerControllerRef;

#pragma endregion

#pragma region Stamina Parameters
	UPROPERTY(EditAnywhere, Category = "Stamina Parameters")
	int Stamina = 500;

	UPROPERTY(EditAnywhere, Category = "Stamina Parameters")
	float ReloadStamina = 5.0f;

	UPROPERTY(EditAnywhere, Category = "Stamina Parameters")
	float DownloadStamina = 1.0f;

	UPROPERTY(EditAnywhere, Category = "Stamina Parameters")
	float StaminaJump = 5;

	FTimerHandle TimerStaminaDecrement;

	FTimerHandle TimerStaminaIncrement;
#pragma endregion

protected:

#pragma region Movement Function

	void MoveForward(float Value);

	void MoveRight(float Value);

	void TurnAtRate(float Rate);

	void LookUpAtRate(float Rate);

#pragma endregion

protected:

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void BeginPlay() override;

public:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	float BaseTurnRate;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	float BaseLookUpRate;

	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	UFUNCTION()
	void AttackOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void AttackOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	void ChangeSpeed(float Speed, float Duration);

	UFUNCTION()
	void BlockRotation();

	UFUNCTION()
	void SpeedReset();

	UFUNCTION()
	void Sprint();

	UFUNCTION()
	void StopSprint();

	UFUNCTION()
	void Attack();

	UFUNCTION()
	void CanAttack();

	UFUNCTION()
	void DecrementStamina();

	UFUNCTION()
	void IncrementStamina();

	UFUNCTION()
	void OpenPannelCrafting();

	UFUNCTION()
	void ClosePannelCrafting();

	UFUNCTION()
	void ScrollUp();

	UFUNCTION()
	void ScrollDown();
};

