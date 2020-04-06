// Fill out your copyright notice in the Description page of Project Settings.


#include "CraftingWidget.h"
#include "Kismet/GameplayStatics.h"

void UCraftingWidget::NativeConstruct()
{
	Super::NativeConstruct();

	delegateMaster = Cast<ADelegateMaster>(UGameplayStatics::GetActorOfClass(GetWorld(), ADelegateMaster::StaticClass()));

	delegateMaster->CraftingFall.BindUObject(this, &UCraftingWidget::ResetIndex);
}

void UCraftingWidget::CallCheckInfoTrap(int index)
{
	delegateMaster->InfoTrap.ExecuteIfBound(delegateMaster->IndexTexture);
}

void UCraftingWidget::ResetIndex()
{
	delegateMaster->IndexTexture = CurrentIndex;
}