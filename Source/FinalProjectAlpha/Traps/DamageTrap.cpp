// Fill out your copyright notice in the Description page of Project Settings.


#include "DamageTrap.h"
#include "Components/DecalComponent.h"
#include "Components/BoxComponent.h"
#include "Boss.h"
#include "NewMinion.h"
#include "FinalProjectAlphaCharacter.h"

// Sets default values
ADamageTrap::ADamageTrap()
{
	Collider = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	RootComponent = Collider;
	Collider->OnComponentBeginOverlap.AddDynamic(this, &ADamageTrap::OnOverlapBegin);

	Decal = CreateDefaultSubobject<UDecalComponent>(TEXT("Decal"));
	Decal->SetupAttachment(Collider);
}

// Called when the game starts or when spawned
void ADamageTrap::BeginPlay()
{
	Super::BeginPlay();

}

void ADamageTrap::OnOverlapBegin(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherComp->ComponentHasTag("BossCollider"))
	{
		UE_LOG(LogTemp, Warning, TEXT("Boss overlap with Damage Trap"))

		ABoss* Boss = Cast<ABoss>(OtherActor);
		if (Boss)
		{
			Boss->HP -= Damage;
			Boss->bBossDamage = true;
			Boss->ControlBoolTrapBoss();
			Boss->CalculateDamage(Damage);
		}	
		Destroy();
	}

	else if (OtherComp->ComponentHasTag("MinionCollider"))
	{
		ANewMinion* Minion = Cast<ANewMinion>(OtherActor);
		if (Minion)
		{
			Minion->CalculateDamage(Damage);
		}
		Destroy();
	}

	else if (OtherComp->ComponentHasTag("PlayerCollider"))
	{
		AFinalProjectAlphaCharacter* Player = Cast<AFinalProjectAlphaCharacter>(OtherActor);
		if (Player)
		{
			Player->TakeDamage(Damage);
		}
		Destroy();
	}
}

