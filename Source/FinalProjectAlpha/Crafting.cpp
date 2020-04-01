// Fill out your copyright notice in the Description page of Project Settings.


#include "Crafting.h"
#include "BackpackPL.h"
#include "GameFramework/Actor.h"
#include "Delegate.h"
#include "DamageTrap.h"
#include "StunTrap.h"
#include "SlowTrap.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UCrafting::UCrafting()
{
	PrimaryComponentTick.bCanEverTick = true;

}


// Called when the game starts
void UCrafting::BeginPlay()
{
	Super::BeginPlay();

	BackpackRef = (UBackpackPL*)GetOwner()->GetComponentByClass(UBackpackPL::StaticClass());
	
	variableRef = Cast<AVariableWidgetRef>(UGameplayStatics::GetActorOfClass(GetWorld(), AVariableWidgetRef::StaticClass()));
	delegateMaster = Cast<ADelegateMaster>(UGameplayStatics::GetActorOfClass(GetWorld(), ADelegateMaster::StaticClass()));

	delegateMaster->InfoTrap.BindUObject(this, &UCrafting::ControllTrap);
}


// Called every frame
void UCrafting::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UCrafting::SetTrap(int indexActor)
{
	TrapSelected = AllActorTrap[indexActor];
	PlaceTrap();
}

void UCrafting::PlaceTrap()
{
	//TrapCrafting();
}

void UCrafting::ControllTrap(int index)
{
	if (index == 0) 
	{
		if (BackpackRef->MagicEssence >= 2) 
		{
			BackpackRef->MagicEssence -= 2;
			delegateMaster->AddTrap.ExecuteIfBound();
			delegateMaster->SwitchImage.ExecuteIfBound();
			UE_LOG(LogTemp,Warning,TEXT("Trap create is Damage"))
		}
		else 
		{
			delegateMaster->CraftingFall.ExecuteIfBound();
		}
	}
	else if (index == 1) 
	{
		if (BackpackRef->MagicEssence >= 2)
		{
			BackpackRef->MagicEssence -=2;
			delegateMaster->SwitchImage.ExecuteIfBound();
			delegateMaster->AddTrap.ExecuteIfBound();
			UE_LOG(LogTemp, Warning, TEXT("Trap create is Stun"))
		}
		else
		{
			delegateMaster->CraftingFall.ExecuteIfBound();
		}
	}
	else
	{
		if (BackpackRef->MagicEssence >= 2)
		{
			BackpackRef->MagicEssence -= 2;
			delegateMaster->SwitchImage.ExecuteIfBound();
			delegateMaster->AddTrap.ExecuteIfBound();
			UE_LOG(LogTemp, Warning, TEXT("Trap create is Slow"))
		}
		else
		{
			delegateMaster->CraftingFall.ExecuteIfBound();
		}
	}
}