// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Utility/DelegateMaster.h"
#include "GameFramework/Character.h"
#include "BackpackPL.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FINALPROJECTALPHA_API UBackpackPL : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UBackpackPL();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int MagicEssence = 10;

	UPROPERTY(VisibleAnywhere)
	TMap<FString, int> CounterTrap;

	UPROPERTY(VisibleAnywhere)
	int counterDamage = 2;

	UPROPERTY(VisibleAnywhere)
	int counterStun = 2;

	UPROPERTY(VisibleAnywhere)
	int counterSlow = 2;

	class ADelegateMaster* delegateMaster;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION()
	void AddTrapInInventory();
};
