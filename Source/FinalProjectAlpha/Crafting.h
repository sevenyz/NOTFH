// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameFramework/Character.h"
#include "VariableWidgetRef.h"
#include "Crafting.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FINALPROJECTALPHA_API UCrafting : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCrafting();

	UPROPERTY(VisibleAnywhere)
	class UBackpackPL* BackpackRef;

	UPROPERTY(VisibleAnywhere)
	class ADelegateMaster* delegateMaster;

	UPROPERTY(VisibleAnywhere)
	class AVariableWidgetRef* variableRef;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "HUD")
	TSubclassOf<class AActor> TrapSelected;

	UPROPERTY(EditAnywhere)
	TArray<TSubclassOf<AActor>> AllActorTrap;

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
};
