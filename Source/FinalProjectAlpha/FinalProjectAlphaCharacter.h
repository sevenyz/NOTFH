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

#pragma region Attack Components

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UBoxComponent* AttackCollider;

#pragma endregion

#pragma region Trap Parameters

	UPROPERTY()
	FTimerHandle TrapTimerHandle;

	UPROPERTY()
	bool CraftingMode;

#pragma endregion

#pragma region Player Parameters

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Parameters")
	int MaxHP = 100;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Parameters")
	int CurrentHP = MaxHP;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Parameters")
	int Damage = 20;

	UPROPERTY(EditAnywhere, Category = "Parameters")
	float MaxSprint = 700.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Parameters")
	float NormalSpeed = 600.0f;

	UPROPERTY(EditAnywhere, Category = "Parameters")
	float MaxRotation = 540.0f;

	UPROPERTY()
	bool bBossInArea = false;

	UPROPERTY()
	bool bMinionArea = false;

	UPROPERTY(BlueprintReadWrite)
	bool bCanAttack = true;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bIsLocked;

	UPROPERTY(BlueprintReadWrite)
	bool bIsCraftingPanelOpen = false;

	class ABoss* BossRef;

	class ANewMinion* minionRef;

	class ADelegateMaster* delegateMaster;

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

	// Implementato anche in BP
	UFUNCTION()
	void AttackOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// Implementato anche in BP
	UFUNCTION()
	void AttackOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	void ChangeSpeed(float Speed, float Duration);

	// forse da levare
	UFUNCTION()
	void BlockRotation();

	UFUNCTION()
	void SpeedReset();

	// implementato anche in BP
	UFUNCTION()
	void Sprint();

	// implementato anche in BP
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

	UFUNCTION(BlueprintImplementableEvent)
	void TakeDamage(int DamageToTake);

	UFUNCTION()
    void CreateTrap();
};

