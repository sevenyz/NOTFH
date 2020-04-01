// Fill out your copyright notice in the Description page of Project Settings.


#include "CraftingWidget.h"
#include "Kismet/GameplayStatics.h"
#include "VariableWidgetRef.h"

void UCraftingWidget::NativeConstruct()
{
	Super::NativeConstruct();

	variableRef = Cast<AVariableWidgetRef>(UGameplayStatics::GetActorOfClass(GetWorld(), AVariableWidgetRef::StaticClass()));
	delegateMaster = Cast<ADelegateMaster>(UGameplayStatics::GetActorOfClass(GetWorld(), ADelegateMaster::StaticClass()));

	delegateMaster->CraftingFall.BindUObject(this, &UCraftingWidget::ResetIndex);
}

void UCraftingWidget::CallCheckInfoTrap(int index)
{
	delegateMaster->InfoTrap.ExecuteIfBound(variableRef->IndexTexture);
}

void UCraftingWidget::ResetIndex()
{
	variableRef->IndexTexture = CurrentIndex;
}


