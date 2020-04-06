// Fill out your copyright notice in the Description page of Project Settings.


#include "GameWidget.h"
#include "Kismet/GameplayStatics.h"

void UGameWidget::NativeConstruct()
{
	Super::NativeConstruct();

	delegateMaster = Cast<ADelegateMaster>(UGameplayStatics::GetActorOfClass(GetWorld(), ADelegateMaster::StaticClass()));

	if (delegateMaster)
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
		ImageTrapSel->SetBrushFromTexture(delegateMaster->AllTextureTrap[delegateMaster->IndexTexture], true);
	}

	if (delegateMaster->bImageUp)
	{
		if (delegateMaster->IndexTexture == 2)
		{
			delegateMaster->IndexTexture = 0;
		}
		else
		{
			delegateMaster->IndexTexture++;
		}

		ImageTrapSel->SetBrushFromTexture(delegateMaster->AllTextureTrap[delegateMaster->IndexTexture], true);
		delegateMaster->bImageUp = false;
	}

	else if (delegateMaster->bImageDown)
	{
		if (delegateMaster->IndexTexture == 0)
		{
			delegateMaster->IndexTexture = 2;
		}
		else
		{
			delegateMaster->IndexTexture--;
			ImageTrapSel->SetBrushFromTexture(delegateMaster->AllTextureTrap[delegateMaster->IndexTexture], true);
		}

		ImageTrapSel->SetBrushFromTexture(delegateMaster->AllTextureTrap[delegateMaster->IndexTexture], true);
		delegateMaster->bImageDown = false;
	}
	else
	{
		ImageTrapSel->SetBrushFromTexture(delegateMaster->AllTextureTrap[delegateMaster->IndexTexture], true);
	}
}
