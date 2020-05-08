// Fill out your copyright notice in the Description page of Project Settings.


#include "Crafting.h"
#include "Utility/DelegateMaster.h"
#include "GameFramework/Actor.h"
#include "Player/BackpackPL.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UCrafting::UCrafting()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UCrafting::BeginPlay()
{
	Super::BeginPlay();

	BackpackRef = (UBackpackPL*)GetOwner()->GetComponentByClass(UBackpackPL::StaticClass());

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
		if (BackpackRef->MagicEssence >= DamageTrapCost)
		{
			BackpackRef->MagicEssence -= DamageTrapCost;
			delegateMaster->AddTrap.ExecuteIfBound();
			delegateMaster->SwitchImage.ExecuteIfBound();
			UE_LOG(LogTemp, Warning, TEXT("Trap create is Damage"))
		}
		else
		{
			delegateMaster->CraftingFall.ExecuteIfBound();
		}
	}
	else if (index == 1)
	{
		if (BackpackRef->MagicEssence >= SlowTrapCost)
		{
			BackpackRef->MagicEssence -= SlowTrapCost;
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
		if (BackpackRef->MagicEssence >= StunTrapCost)
		{
			BackpackRef->MagicEssence -= StunTrapCost;
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