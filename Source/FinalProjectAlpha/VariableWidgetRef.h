// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/Texture.h"
#include "DelegateMaster.h"
#include "VariableWidgetRef.generated.h"


UCLASS()
class FINALPROJECTALPHA_API AVariableWidgetRef : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AVariableWidgetRef();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
	int32 IndexTexture;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
	TArray<class UTexture2D*> AllTextureTrap;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
