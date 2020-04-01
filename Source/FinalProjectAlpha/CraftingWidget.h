// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BackpackPL.h"
#include "DelegateMaster.h"
#include "Components/Button.h"
#include "CraftingWidget.generated.h"


UCLASS()
class FINALPROJECTALPHA_API UCraftingWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	virtual void NativeConstruct() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "HUD")
	class AVariableWidgetRef* variableRef;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "HUD")
	class ADelegateMaster* delegateMaster;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
	int CurrentIndex;

	UFUNCTION(BlueprintCallable, Category = "Function")
	void CallCheckInfoTrap(int index);

	UFUNCTION()
	void ResetIndex();
};
