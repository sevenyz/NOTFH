#include "BackpackPL.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"

// Sets default values for this component's properties
UBackpackPL::UBackpackPL()
{
	PrimaryComponentTick.bCanEverTick = true;

}


// Called when the game starts
void UBackpackPL::BeginPlay()
{
	Super::BeginPlay();

	variableRef = Cast<AVariableWidgetRef>(UGameplayStatics::GetActorOfClass(GetWorld(), AVariableWidgetRef::StaticClass()));
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

}

void UBackpackPL::AddTrapInInventory()
{
	if (variableRef->IndexTexture == 0) 
	{
		CounterTrap.Add(TEXT("Damage"), counterDamage++);
	}
	else if(variableRef->IndexTexture == 1)
	{
		CounterTrap.Add(TEXT("Slow"), counterSlow++);
	}
	else
	{
		CounterTrap.Add(TEXT("Stun"), counterStun++);
	}
}

