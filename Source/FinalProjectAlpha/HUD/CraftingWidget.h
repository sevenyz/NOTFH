// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Player/BackpackPL.h"
#include "Utility/DelegateMaster.h"
#include "CraftingWidget.generated.h"


UCLASS()
class FINALPROJECTALPHA_API UCraftingWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	virtual void NativeConstruct() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "HUD")
	class ADelegateMaster* delegateMaster;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
	int CurrentIndex;

	UFUNCTION(BlueprintCallable, Category = "Function")
	void CallCheckInfoTrap(int index);

	UFUNCTION()
	void ResetIndex();
};
