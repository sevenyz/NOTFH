// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss.h"
#include "Engine/World.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "TimerManager.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Components/BoxComponent.h"
#include "FinalProjectAlphaCharacter.h"


// Sets default values
ABoss::ABoss()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AttackCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("AttackCollider"));
	AttackCollider->SetupAttachment(RootComponent);

	AttackCollider->OnComponentBeginOverlap.AddDynamic(this, &ABoss::AttackOverlap);
	AttackCollider->OnComponentEndOverlap.AddDynamic(this, &ABoss::AttackOverlapEnd);

}

// Called when the game starts or when spawned
void ABoss::BeginPlay()
{
	Super::BeginPlay();

	AAIController* AIController = UAIBlueprintHelperLibrary::GetAIController(this);
	AIController->UseBlackboard(BlackboardToUse, Blackboard);
}

// Called every frame
void ABoss::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABoss::ChangeSpeed(float Speed, float Duration)
{
	GetCharacterMovement()->MaxWalkSpeed = Speed;
	StunTime = Duration;

	TrapTimerHandle.Invalidate();
	GetWorld()->GetTimerManager().SetTimer(TrapTimerHandle, this, &ABoss::SpeedReset, Duration, false);
}

void ABoss::SpeedReset()
{
	GetCharacterMovement()->MaxWalkSpeed = 400.f;

	this->bUseControllerRotationYaw = true;
	bIsStunned = false;

	if (Blackboard->HasValidAsset())
	{
		Blackboard->SetValueAsBool(IsStunnedKeyName, false);
		UE_LOG(LogTemp, Warning, TEXT("set value false"))
	}
}

void ABoss::BlockRotation()
{
	this->bUseControllerRotationYaw = false;
	bIsStunned = true;

	if (Blackboard->HasValidAsset())
	{
		Blackboard->SetValueAsBool(IsStunnedKeyName, true);
		UE_LOG(LogTemp, Warning, TEXT("set value true"))
	}
}

void ABoss::DamageCalculation()
{
	if (bIsStunned)
	{
		UE_LOG(LogTemp, Warning, TEXT("Boss take damage"));
	}
}

void ABoss::Berserk()
{
	BerserkMood = true;
	UE_LOG(LogTemp, Warning, TEXT("Berserk mood on"));
	GetCharacterMovement()->MaxWalkSpeed = VelocityMovementBerserkMood;
	GetWorld()->GetTimerManager().SetTimer(TimerBerserkMood, this, &ABoss::ResetBoolTrapBoss, true, TimerDurationBerserkMood);
}

void ABoss::ControlBoolTrapBoss()
{
	if (BossStun) {
	GetWorld()->GetTimerManager().SetTimer(TimerControlBoolStun, this, &ABoss::ResetBoolTrapBoss, true, TimerCheck);
	}
	else if (BossDamage) {
	GetWorld()->GetTimerManager().SetTimer(TimerControlBoolDamage, this, &ABoss::ResetBoolTrapBoss, true, TimerCheck);
	}
}

void ABoss::ResetBoolTrapBoss()
{
	if (BossStun) {
		BossStun = false;
		GetWorld()->GetTimerManager().ClearTimer(TimerControlBoolStun);
	}
	if (BossDamage) {
		BossDamage = false;
		GetWorld()->GetTimerManager().ClearTimer(TimerControlBoolDamage);
	}
	else
	{
		//SpeedReset();
		BerserkMood = false;
		GetWorld()->GetTimerManager().ClearTimer(TimerBerserkMood);
	}
}

void ABoss::AttackOverlap(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
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

void ABoss::AttackOverlapEnd(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	bCanAttack = false;
}

void ABoss::Attack()
{
	if (bCanAttack)
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerTakeDamage"))
	}
}

