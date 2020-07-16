// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Player/BackpackPL.h"
#include "Utility/DelegateMaster.h"
#include "FinalProjectAlphaCharacter.generated.h"

UENUM()
enum EGrimadeSelection
{
    Damage,
    Stun,
    Slow,
};

USTRUCT(BlueprintType)
struct FGrimadeStats
{
public:
    GENERATED_USTRUCT_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, BlueprintType)
    TEnumAsByte<EGrimadeSelection> GrimadeSelection;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 GrimadeCost;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 CraftedGrimadesCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 GrimadesMaxCount;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 GrimadeIndex;

};

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray <FGrimadeStats> GrimadeArray;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FGrimadeStats SelectedGrimade;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 SelectedGrimadeIndex;

#pragma endregion

#pragma region Player Parameters

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Parameters")
	float MaxHP = 100;

	UPROPERTY(BlueprintReadWrite, Category = "Parameters")
	float CurrentHP = MaxHP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Parameters")
	int Damage = 20;

	UPROPERTY(EditAnywhere, Category = "Parameters")
	float MaxSprint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Parameters")
	float NormalSpeed;

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

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bCanSprint = true;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bCanDodge = true;

	UPROPERTY(BlueprintReadWrite)
	bool bIsCraftingPanelOpen = false;

	UPROPERTY(BlueprintReadWrite)
	bool bIsStunned = false;

	UPROPERTY(BlueprintReadWrite)
	bool bIsBloodDrainActive = false;

	class ABoss* BossRef;

	class ANewMinion* minionRef;

	class ADelegateMaster* delegateMaster;

	class AMyPlayerController* PlayerControllerRef;

#pragma endregion

#pragma region Stamina Parameters

	//Aggiunto, necessario per il calcolo della percentuale della progress bar
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stamina Parameters")
	int MaxStamina = 50;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stamina Parameters")
	int Stamina = 50;

	//Aggiunti per nuova funzione SPRINT
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stamina Parameters")
	float StaminaRefillRate = 0.2f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stamina Parameters")
	float StaminaDrainRate = 0.1f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stamina Parameters")
	float StaminaRefillWaitTime = 2.f;

	FTimerHandle StaminaRefillTimerHandle;
	FTimerHandle StaminaDrainTimerHandle;

	//fine elementi aggiunti

	/*
	UPROPERTY(EditAnywhere, Category = "Stamina Parameters")			//Obsoleto
	float ReloadStamina = 5.0f;

	UPROPERTY(EditAnywhere, Category = "Stamina Parameters")			//Obsoleto
	float DownloadStamina = 1.0f;

	UPROPERTY(EditAnywhere, Category = "Stamina Parameters")			//Obsoleto
	float StaminaJump = 5;

	FTimerHandle TimerStaminaDecrement;									//Obsoleto

	FTimerHandle TimerStaminaIncrement;									//Obsoleto
	*/
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

	UFUNCTION(BlueprintCallable)
	void ClearStaminaDrainTimer();

	UFUNCTION()
	void Attack();

	UFUNCTION()
	void CanAttack();

	//UFUNCTION()
	//void DecrementStamina();	//Obsoleta

	//UFUNCTION()
	//void IncrementStamina();	//Obsoleta

	UFUNCTION()
	void RefillStamina(); // aggiunto per nuova funzione SPRINT
	UFUNCTION()
	void DrainStamina(); // aggiunto per nuova funzione SPRINT



	UFUNCTION()
	void OpenPannelCrafting();

	UFUNCTION()
	void ClosePannelCrafting();

	UFUNCTION()
	void ScrollUp();

	UFUNCTION()
	void ScrollDown();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void TakeDamage(float DamageToTake);

	UFUNCTION()
    void CreateTrap();
};

