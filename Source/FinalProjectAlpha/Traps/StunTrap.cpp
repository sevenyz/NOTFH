// Fill out your copyright notice in the Description page of Project Settings.


#include "StunTrap.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "FinalProjectAlphaCharacter.h"
#include "Boss.h"
#include "NewMinion.h"

// Sets default values
AStunTrap::AStunTrap()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Collider = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));

	RootComponent = Mesh;
	Collider->SetupAttachment(RootComponent);

	Collider->OnComponentBeginOverlap.AddDynamic(this, &AStunTrap::OnOverlapBegin);

}

// Called when the game starts or when spawned
void AStunTrap::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AStunTrap::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

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
	}

	else if (OtherActor->ActorHasTag("Minion"))
	{
		ANewMinion* Minion = Cast<ANewMinion>(OtherActor);
		if (Minion) 
		{
			Minion->ChangeSpeed(0, StunTime);
			Minion->BlockRotation();
		}
	}

	else if (OtherActor->ActorHasTag("Player"))
	{
		AFinalProjectAlphaCharacter* Player = Cast<AFinalProjectAlphaCharacter>(OtherActor);
		if (Player)
		{
			Player->ChangeSpeed(0, StunTime);
			Player->BlockRotation();
		}
	}

	Destroy();
}
