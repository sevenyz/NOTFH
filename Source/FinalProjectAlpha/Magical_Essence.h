// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Magical_Essence.generated.h"

UCLASS()
class FINALPROJECTALPHA_API AMagical_Essence : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMagical_Essence();

	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* Mesh;

	UPROPERTY(VisibleAnywhere)
	class UBoxComponent* Collider;

	UPROPERTY(EditAnywhere, Category = "Rotation Object")
	float PitchValue;

	UPROPERTY(EditAnywhere, Category = "Rotation Object")
	float YawValue;

	UPROPERTY(EditAnywhere, Category = "Rotation Object")
	float RollValue;

	UPROPERTY(VisibleAnywhere)
	class UBackpackPL* BackpackRef;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void MoveObject();

};
