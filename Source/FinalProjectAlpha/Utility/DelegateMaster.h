// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DelegateMaster.generated.h"

DECLARE_DELEGATE(FuncSwitchImage)
//DECLARE_DELEGATE_OneParam(GetPositionTrap, FHitResult)
DECLARE_DELEGATE_OneParam(CheckInfoTrap, int)
DECLARE_DELEGATE(CraftingTrapFail)
DECLARE_DELEGATE(AddTrapToInventory)
DECLARE_DELEGATE_OneParam(InputCreateTrap, int)

UCLASS()
class FINALPROJECTALPHA_API ADelegateMaster : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADelegateMaster();

	FuncSwitchImage SwitchImage;

	CheckInfoTrap InfoTrap;

	CraftingTrapFail CraftingFall;

	AddTrapToInventory AddTrap;

	InputCreateTrap CreateTrap;

	UPROPERTY()
	bool bImageUp;

	UPROPERTY()
	bool bImageDown;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
	int32 IndexTexture;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
	TArray<class UTexture2D*> AllTextureTrap;	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
