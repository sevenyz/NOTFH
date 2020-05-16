// Fill out your copyright notice in the Description page of Project Settings.


#include "SlowTrap.h"
#include "Components/DecalComponent.h"
#include "Components/BoxComponent.h"
#include "FinalProjectAlphaCharacter.h"
#include "GameFramework/Actor.h"
#include "Boss.h"
#include "NewMinion.h"

// Sets default values
ASlowTrap::ASlowTrap()
{
	Collider = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	RootComponent = Collider;
	Collider->OnComponentBeginOverlap.AddDynamic(this, &ASlowTrap::OnOverlapBegin);

	Decal = CreateDefaultSubobject<UDecalComponent>(TEXT("Decal"));
	Decal->SetupAttachment(Collider);
}

// Called when the game starts or when spawned
void ASlowTrap::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASlowTrap::OnOverlapBegin(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherActor->ActorHasTag("Boss"))
	{
		ABoss* Boss = Cast<ABoss>(OtherActor);
		if (Boss && Boss->bBerserkMood == false)
		{
			Boss->ChangeSpeed(100, SlowTime);
		}
		Destroy();
	}

	else if (OtherActor->ActorHasTag("Minion"))
	{
		ANewMinion* Minion = Cast<ANewMinion>(OtherActor);
		if (Minion)
		{
			Minion->ChangeSpeed(100, SlowTime);
		}
		Destroy();
	}

	else if (OtherComp->ComponentHasTag("PlayerCollider"))
	{
		AFinalProjectAlphaCharacter* Player = Cast<AFinalProjectAlphaCharacter>(OtherActor);
		if (Player)
		{
			Player->ChangeSpeed(300, SlowTime);
		}	
		Destroy();
	}
}