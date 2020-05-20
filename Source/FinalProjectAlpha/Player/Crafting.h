// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameFramework/Character.h"
#include "Crafting.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FINALPROJECTALPHA_API UCrafting : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCrafting();

	class UBackpackPL* BackpackRef;

	class ADelegateMaster* delegateMaster;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TSubclassOf<class AActor> TrapSelected;

	UPROPERTY(EditAnywhere)
	TArray<TSubclassOf<AActor>> AllActorTrap;

	UPROPERTY(EditAnywhere, Category = "TrapCost")
	int32 DamageTrapCost;

	UPROPERTY(EditAnywhere, Category = "TrapCost")
	int32 SlowTrapCost;

	UPROPERTY(EditAnywhere, Category = "TrapCost")
	int32 StunTrapCost;

	int* MinValue = nullptr;

	UPROPERTY(EditAnywhere, Category = "Distance trap")
    float SpawnDistance = 200.f;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION()
	void SetTrap(int indexActor);

	UFUNCTION()
	void PlaceTrap();

	UFUNCTION()
	void ControllTrap(int index);

	FString ControlStringInventory(int index);
};
