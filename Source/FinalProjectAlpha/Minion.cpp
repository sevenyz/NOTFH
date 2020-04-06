// Fill out your copyright notice in the Description page of Project Settings.


#include "Minion.h"
#include "FinalProjectAlphaCharacter.h"
#include "TimerManager.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"

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

	AAIController* AIController = UAIBlueprintHelperLibrary::GetAIController(this);
	AIController->UseBlackboard(BlackboardToUse, Blackboard);
	
}

// Called every frame
void AMinion::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMinion::ChangeSpeed(float Speed, float Duration)
{
	PawnMovement->MaxSpeed = Speed;
	StunTime = Duration;

	TrapTimerHandle.Invalidate();
	GetWorld()->GetTimerManager().SetTimer(TrapTimerHandle, this, &AMinion::SpeedReset, Duration, false);
}

void AMinion::SpeedReset()
{
	PawnMovement->MaxSpeed = 400.f;
	this->bUseControllerRotationYaw = true;

	if (Blackboard->HasValidAsset())
	{
		Blackboard->SetValueAsBool(IsStunnedKeyName, false);
		UE_LOG(LogTemp, Warning, TEXT("set value false"))
	}
}

void AMinion::BlockRotation()
{	
	this->bUseControllerRotationYaw = false;

	if (Blackboard->HasValidAsset())
	{
		Blackboard->SetValueAsBool(IsStunnedKeyName, true);
		UE_LOG(LogTemp, Warning, TEXT("set value true"))
	}
}

void AMinion::AttackOverlap(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherActor->ActorHasTag("Player"))
	{
		PlayerRef = Cast<AFinalProjectAlphaCharacter>(OtherActor);

		if (PlayerRef)
		{
			bCanAttack = true;
		}
	}
}

void AMinion::AttackOverlapEnd(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	bCanAttack = false;
}

void AMinion::Attack()
{
	if (bCanAttack)
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerTakeDamage"))
	}
}

void AMinion::CalculateDamage(int damageDirect)
{

	HP -= damageDirect;

	if (HP <= 0)
	{
		FVector locationToSpawn = (GetActorLocation() + FVector(0.0f, 0.0f, 50.0f));
		FRotator rotatorToSpawn = FRotator::ZeroRotator;

		GetWorld()->SpawnActor(MagicalEssence, &locationToSpawn, &rotatorToSpawn);
		Destroy(this);
	}
}