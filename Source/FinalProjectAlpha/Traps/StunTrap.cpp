// Fill out your copyright notice in the Description page of Project Settings.


#include "StunTrap.h"
#include "Components/DecalComponent.h"
#include "Components/BoxComponent.h"
#include "FinalProjectAlphaCharacter.h"
#include "Boss.h"
#include "NewMinion.h"

// Sets default values
AStunTrap::AStunTrap()
{
	Collider = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	RootComponent = Collider;
	Collider->OnComponentBeginOverlap.AddDynamic(this, &AStunTrap::OnOverlapBegin);

	Decal = CreateDefaultSubobject<UDecalComponent>(TEXT("Decal"));
	Decal->SetupAttachment(Collider);
}

// Called when the game starts or when spawned
void AStunTrap::BeginPlay()
{
	Super::BeginPlay();
	
}

void AStunTrap::OnOverlapBegin(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherActor->ActorHasTag("Boss"))
	{
		ABoss* Boss = Cast<ABoss>(OtherActor);
		if (Boss && Boss->bBerserkMood == false)
		{
			Boss->ChangeSpeed(0, StunTime);
			Boss->BlockRotation();
			Boss->bBossStun = true;
			Boss->ControlBoolTrapBoss();
		}
		Destroy();
	}

	else if (OtherActor->ActorHasTag("Minion"))
	{
		ANewMinion* Minion = Cast<ANewMinion>(OtherActor);
		if (Minion) 
		{
			Minion->ChangeSpeed(0, StunTime);
			Minion->BlockRotation();
		}
		Destroy();
	}

	else if (OtherComp->ComponentHasTag("PlayerCollider"))
	{
		AFinalProjectAlphaCharacter* Player = Cast<AFinalProjectAlphaCharacter>(OtherActor);
		if (Player)
		{
			UE_LOG(LogTemp, Warning, TEXT("Collision"))
			Player->ChangeSpeed(0, StunTime);
			Player->BlockRotation();
		}
		Destroy();
	}
}
