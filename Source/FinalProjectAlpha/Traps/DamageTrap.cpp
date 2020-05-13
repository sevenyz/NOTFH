#include "DamageTrap.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Boss.h"
#include "NewMinion.h"

// Sets default values
ADamageTrap::ADamageTrap()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Collider = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));

	RootComponent = Mesh;
	Collider->SetupAttachment(RootComponent);

	Collider->OnComponentBeginOverlap.AddDynamic(this, &ADamageTrap::OnOverlapBegin);

}

// Called when the game starts or when spawned
void ADamageTrap::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ADamageTrap::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADamageTrap::OnOverlapBegin(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherActor->ActorHasTag("Boss"))
	{
		UE_LOG(LogTemp, Warning, TEXT("Boss overlap with Damage Trap"))

		ABoss* Boss = Cast<ABoss>(OtherActor);
		if (Boss && Boss->bBerserkMood == false)
		{
			Boss->HP -= Damage;
			Boss->bBossDamage = true;
			Boss->ControlBoolTrapBoss();
		}	
	}

	else if (OtherActor->ActorHasTag("Minion"))
	{
		ANewMinion* Minion = Cast<ANewMinion>(OtherActor);
		if (Minion)
		{
			//Minion->HP -= Damage;
			Minion->CalculateDamage(Damage);
		}
	}

	Destroy();
}

