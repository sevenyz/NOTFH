// Fill out your copyright notice in the Description page of Project Settings.


#include "GrimadeBase.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
AGrimadeBase::AGrimadeBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Collider = CreateDefaultSubobject<USphereComponent>(TEXT("Collider"));
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	ProjectileComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComponent"));

	RootComponent = Collider;

	Mesh->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void AGrimadeBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGrimadeBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

