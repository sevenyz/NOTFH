
#pragma once
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "VariableWidgetRef.h"
#include "DelegateMaster.h"
#include "GameFramework/Character.h"
#include "BackpackPL.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FINALPROJECTALPHA_API UBackpackPL : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UBackpackPL();

	UPROPERTY(VisibleAnywhere)
	int MagicEssence = 10;

	UPROPERTY(VisibleAnywhere)
	TMap<FString, int32> CounterTrap;

	UPROPERTY(VisibleAnywhere)
	int counterDamage;

	UPROPERTY(VisibleAnywhere)
	int counterStun;

	UPROPERTY(VisibleAnywhere)
	int counterSlow;

	UPROPERTY(VisibleAnywhere)
	class AVariableWidgetRef* variableRef;

	UPROPERTY(VisibleAnywhere)
	class ADelegateMaster* delegateMaster;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION()
	void AddTrapInInventory();

};
