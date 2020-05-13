// Fill out your copyright notice in the Description page of Project Settings.


#include "Magical_Essence.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "ConstructorHelpers.h"
#include "FinalProjectAlphaCharacter.h"
#include "Player/BackpackPL.h"

// Sets default values
AMagical_Essence::AMagical_Essence()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Collider = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	RootComponent = Collider;
	Collider->SetSimulatePhysics(false);
	Collider->SetEnableGravity(true);
	Collider->SetNotifyRigidBodyCollision(true);
	Collider->SetBoxExtent(FVector(20, 20, 20));
	Collider->BodyInstance.SetCollisionProfileName("OverlapAllDynamic");
	Collider->OnComponentBeginOverlap.AddDynamic(this, &AMagical_Essence::OnOverlapBegin);

	ConstructorHelpers::FObjectFinder<UStaticMesh>meshFromAsset(TEXT("StaticMesh'/Game/Environment/Static_Meshes/eyeball.eyeball'"));
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	Mesh->SetStaticMesh(meshFromAsset.Object);
	Mesh->SetupAttachment(RootComponent);

	PitchValue = 0.f;
	YawValue = 1.f;
	RollValue = 1.f;

}

// Called when the game starts or when spawned
void AMagical_Essence::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMagical_Essence::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	MoveObject();
}

void AMagical_Essence::OnOverlapBegin(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherComp->ComponentHasTag("PlayerCollider"))
	{
		AFinalProjectAlphaCharacter* Player = Cast<AFinalProjectAlphaCharacter>(OtherActor);

		BackpackRef = (UBackpackPL*)Player->GetComponentByClass(UBackpackPL::StaticClass());

		if (BackpackRef)
		{
			BackpackRef->MagicEssence++;

			UE_LOG(LogTemp, Warning, TEXT("Magical Essence = %d"), BackpackRef->MagicEssence)

			Destroy(this);
		}
	}
}

void AMagical_Essence::MoveObject()
{
	FRotator NewRotation = FRotator(PitchValue, YawValue, RollValue);

	FQuat QuatRotation = FQuat(NewRotation);

	AddActorLocalRotation(QuatRotation, false, 0, ETeleportType::None);

}