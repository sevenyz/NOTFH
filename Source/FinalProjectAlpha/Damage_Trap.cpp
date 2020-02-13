// Fill out your copyright notice in the Description page of Project Settings.


#include "Damage_Trap.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Boss.h"

// Sets default values
ADamage_Trap::ADamage_Trap()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Collider = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));

	RootComponent = Mesh;
	Collider->SetupAttachment(RootComponent);

	Collider->OnComponentBeginOverlap.AddDynamic(this, &ADamage_Trap::OnOverlapBegin);

}

// Called when the game starts or when spawned
void ADamage_Trap::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADamage_Trap::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADamage_Trap::OnOverlapBegin(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherActor->ActorHasTag("Boss"))
	{
		UE_LOG(LogTemp, Warning, TEXT("Boss overlap with Damage Trap"))

		ABoss* Boss = Cast<ABoss>(OtherActor);
		if (Boss && Boss->BerserkMood == false)
		{
			Boss->HP =- Damage;
			Boss->BossDamage = true;
			Boss->ControlBoolTrapBoss();
		}

		Destroy();
	}
}
