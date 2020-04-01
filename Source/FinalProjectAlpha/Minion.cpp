// Fill out your copyright notice in the Description page of Project Settings.


#include "Minion.h"
#include "FinalProjectAlphaCharacter.h"

// Sets default values
AMinion::AMinion()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MyBox = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	RootComponent = MyBox;

	MyMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MyMesh"));
	MyMesh->SetupAttachment(MyBox);

	ArrowComponent = CreateDefaultSubobject<UArrowComponent>(TEXT("ArrowComponent"));
	ArrowComponent->SetupAttachment(MyBox);

	PawnMovement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("MovementComponent"));

	AttackCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("AttackCollider"));
	AttackCollider->SetupAttachment(RootComponent);

	AttackCollider->OnComponentBeginOverlap.AddDynamic(this, &AMinion::AttackOverlap);
	AttackCollider->OnComponentEndOverlap.AddDynamic(this, &AMinion::AttackOverlapEnd);

}

// Called when the game starts or when spawned
void AMinion::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMinion::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMinion::AttackOverlap(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherActor->ActorHasTag("Player"))
	{
		PlayerRef = Cast<AFinalProjectAlphaCharacter>(OtherActor);

		if (PlayerRef)
		{
			bPlayerInArea = true;
		}
	}
}

void AMinion::AttackOverlapEnd(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	bPlayerInArea = false;
}

void AMinion::Attack()
{
	if (bPlayerInArea)
	{
		//Player->TakeDamage();
	}
}



