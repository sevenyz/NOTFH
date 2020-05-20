// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerController.h"
#include "Blueprint/UserWidget.h"

void AMyPlayerController::BeginPlay()
{

	Super::BeginPlay();

	if (WidgetGame)
	{
		WGameRef = CreateWidget<UUserWidget>(this, WidgetGame);

		if (WGameRef)
		{
			WGameRef->AddToViewport();
		}
	}

}

void AMyPlayerController::OpenCrafting()
{
	if (WGameRef)
	{
		WGameRef->SetVisibility(ESlateVisibility::Hidden);
	}

	if (WCraftingRef == nullptr)
	{
		WCraftingRef = CreateWidget<UUserWidget>(this, WidgetCrafting);

		if (WCraftingRef)
		{
			WCraftingRef->AddToViewport();
		}
		bShowMouseCursor = true;

	}

	else
	{
		WCraftingRef->SetVisibility(ESlateVisibility::Visible);
		bShowMouseCursor = true;
	}

	SetInputMode(FInputModeGameAndUI());

}

void AMyPlayerController::CloseCrafting()
{
	if (WCraftingRef)
	{
		WCraftingRef->SetVisibility(ESlateVisibility::Hidden);
		bShowMouseCursor = false;
	}

	if (WGameRef)
	{
		WGameRef->SetVisibility(ESlateVisibility::Visible);

		bShowMouseCursor = false;
	}

	SetInputMode(FInputModeGameOnly());
}
