// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Engine/Texture.h"
#include "Components/Image.h"
#include "Utility/DelegateMaster.h"
#include "GameWidget.generated.h"

/**
 * 
 */
UCLASS()
class FINALPROJECTALPHA_API UGameWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	virtual void NativeConstruct() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget), Category = "HUD")
	class UImage* ImageTrapSel;

	UPROPERTY(VisibleAnywhere, Category = "HUD")
	class ADelegateMaster* delegateMaster;

	UPROPERTY(VisibleAnywhere, Category = "HUD")
	bool bInitial = false;

public:

	UFUNCTION()
	void ChangeImage();

};
