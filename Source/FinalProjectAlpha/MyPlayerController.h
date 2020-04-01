// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MyPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class FINALPROJECTALPHA_API AMyPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public :

#pragma region HUD Parameters

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
	TSubclassOf<class UUserWidget> WidgetGame;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
	TSubclassOf<class UUserWidget> WidgetCrafting;

	UUserWidget* WCraftingRef;

	UUserWidget* WGameRef;

#pragma endregion

	virtual  void  BeginPlay()  override;

public:

	UFUNCTION()
	void OpenCrafting();

	UFUNCTION()
	void CloseCrafting();
};
