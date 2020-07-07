// Fill out your copyright notice in the Description page of Project Settings.


#include "NewMinion.h"
#include "FinalProjectAlphaCharacter.h"
#include "Magical_Essence.h"
#include "TimerManager.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/BoxComponent.h"

// Sets default values
ANewMinion::ANewMinion()
{
	AttackCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("AttackCollider"));
	AttackCollider->SetupAttachment(RootComponent);
	AttackCollider->OnComponentBeginOverlap.AddDynamic(this, &ANewMinion::AttackOverlap);
	AttackCollider->OnComponentEndOverlap.AddDynamic(this, &ANewMinion::AttackOverlapEnd);
}

// Called when the game starts or when spawned
void ANewMinion::BeginPlay()
{
	Super::BeginPlay();

	AAIController* AIController = UAIBlueprintHelperLibrary::GetAIController(this);
	AIController->UseBlackboard(BlackboardToUse, Blackboard);

	ACharacter* Character = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	if (Character) 
	{
		PlayerRef = Cast<AFinalProjectAlphaCharacter>(Character);
	}
}

void ANewMinion::ChangeSpeed(float Speed, float Duration)
{
	bCanAttack = false;
	GetCharacterMovement()->MaxWalkSpeed = Speed;
	StunTime = Duration;

	TrapTimerHandle.Invalidate();
	GetWorld()->GetTimerManager().SetTimer(TrapTimerHandle, this, &ANewMinion::SpeedReset, Duration, false);
}

void ANewMinion::SpeedReset()
{
	bCanAttack = true;
	GetCharacterMovement()->MaxWalkSpeed = 400.f;
	this->bUseControllerRotationYaw = true;

	if (Blackboard->HasValidAsset())
	{
		Blackboard->SetValueAsBool(IsStunnedKeyName, false);
		UE_LOG(LogTemp, Warning, TEXT("set value false"))
	}
}

void ANewMinion::BlockRotation()
{
	this->bUseControllerRotationYaw = false;

	if (Blackboard->HasValidAsset())
	{
		Blackboard->SetValueAsBool(IsStunnedKeyName, true);
		UE_LOG(LogTemp, Warning, TEXT("set value true"))
	}
}

void ANewMinion::CalculateDamage(int damageDirect)
{
	float DistanceFromPlayer = GetDistanceTo(PlayerRef);
	
	if (DistanceFromPlayer <= 200.f && FMath::RandRange(1, 10) <= StepbackBeforeAttackProbability && !Blackboard->GetValueAsBool(IsStunnedKeyName))
	{
		Blackboard->SetValueAsBool(CanStepbackName, true);			
	}
	else 
	{
		Blackboard->SetValueAsBool(IsHitKeyName, true);
		CurrentHP -= damageDirect * HeatShockDamage;
		bHasTakenDamage = true;
	}
	
	if (CurrentHP <= 0)
	{
		//FVector locationToSpawn = (GetActorLocation() + FVector(0.0f, 0.0f, 50.0f));
		//FRotator rotatorToSpawn = FRotator::ZeroRotator;

		//GetWorld()->SpawnActor(MagicalEssence, &locationToSpawn, &rotatorToSpawn);
		Death();
	}
}

void ANewMinion::AttackOverlap(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherComp->ComponentHasTag("PlayerCollider")) 
	{
		bCanAttack = true;
	}	
}

void ANewMinion::AttackOverlapEnd(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	if (OtherComp->ComponentHasTag("PlayerCollider"))
	{
		bCanAttack = false;
	}
}

void ANewMinion::Attack()
{
	if (bCanAttack)
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerTakeDamage"))
		PlayerRef->TakeDamage(Damage);
	}
}



