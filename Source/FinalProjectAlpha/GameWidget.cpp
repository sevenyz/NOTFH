// Fill out your copyright notice in the Description page of Project Settings.


#include "GameWidget.h"
#include "Kismet/GameplayStatics.h"
#include "VariableWidgetRef.h"

void UGameWidget::NativeConstruct() 
{
	Super::NativeConstruct();
	
	variableRef = Cast<AVariableWidgetRef>(UGameplayStatics::GetActorOfClass(GetWorld(), AVariableWidgetRef::StaticClass()));
	delegateMaster = Cast<ADelegateMaster>(UGameplayStatics::GetActorOfClass(GetWorld(), ADelegateMaster::StaticClass()));

	if (variableRef && delegateMaster)
	{
		ChangeImage();
		delegateMaster->SwitchImage.BindUObject(this, &UGameWidget::ChangeImage);
		bInitial = true;
	}
	
}

void UGameWidget::ChangeImage()
{
	if (!bInitial) 
	{
		ImageTrapSel->SetBrushFromTexture(variableRef->AllTextureTrap[variableRef->IndexTexture], true);
	}

	if (delegateMaster->bImageUp) 
	{
		if (variableRef->IndexTexture == 2) 
		{
			variableRef->IndexTexture = 0;
		}
		else
		{
			variableRef->IndexTexture++;
		}

		ImageTrapSel->SetBrushFromTexture(variableRef->AllTextureTrap[variableRef->IndexTexture], true);
		delegateMaster->bImageUp = false;
	}

	else if(delegateMaster->bImageDown)
	{
		if (variableRef->IndexTexture == 0) 
		{
			variableRef->IndexTexture = 2;
		}
		else 
		{
			variableRef->IndexTexture--;
			ImageTrapSel->SetBrushFromTexture(variableRef->AllTextureTrap[variableRef->IndexTexture], true);
		}

		ImageTrapSel->SetBrushFromTexture(variableRef->AllTextureTrap[variableRef->IndexTexture], true);
		delegateMaster->bImageDown = false;
	}
}