// Fill out your copyright notice in the Description page of Project Settings.


#include "BackpackPL.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"

// Sets default values for this component's properties
UBackpackPL::UBackpackPL()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UBackpackPL::BeginPlay()
{
	Super::BeginPlay();

	delegateMaster = Cast<ADelegateMaster>(UGameplayStatics::GetActorOfClass(GetWorld(), ADelegateMaster::StaticClass()));

	delegateMaster->AddTrap.BindUObject(this, &UBackpackPL::AddTrapInInventory);

	CounterTrap.Add(TEXT("Stun"), counterStun);
	CounterTrap.Add(TEXT("Slow"), counterSlow);
	CounterTrap.Add(TEXT("Damage"), counterDamage);
	
}


// Called every frame
void UBackpackPL::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UBackpackPL::AddTrapInInventory()
{
	if (delegateMaster->IndexTexture == 0)
	{
		CounterTrap.Add(TEXT("Damage"), counterDamage++);
	}
	else if (delegateMaster->IndexTexture == 1)
	{
		CounterTrap.Add(TEXT("Slow"), counterSlow++);
	}
	else
	{
		CounterTrap.Add(TEXT("Stun"), counterStun++);
	}
}