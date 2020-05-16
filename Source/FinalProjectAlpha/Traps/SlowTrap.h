// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SlowTrap.generated.h"

UCLASS()
class FINALPROJECTALPHA_API ASlowTrap : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASlowTrap();

	UPROPERTY(VisibleAnywhere)
	class UDecalComponent* Decal;

	UPROPERTY(VisibleAnywhere)
	class UBoxComponent* Collider;

	UPROPERTY(EditAnywhere, Category = "Info Craft")
	int Cost = 2;

	UPROPERTY(EditAnywhere, Category = "Info Craft")
	float TimeCraft = 3.0f;

	UPROPERTY(EditAnywhere, Category = "Settings")
	float SlowTime = 2.f;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);


};
