// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "FinalProjectAlphaCharacter.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/BoxComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "DrawDebugHelpers.h"
#include "TimerManager.h"
#include "Boss.h"

//////////////////////////////////////////////////////////////////////////
// AFinalProjectAlphaCharacter

AFinalProjectAlphaCharacter::AFinalProjectAlphaCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)

	LineTraceStartPoint = CreateDefaultSubobject<USceneComponent>(TEXT("LineTraceStartPoint"));
	LineTraceEndPoint = CreateDefaultSubobject<USceneComponent>(TEXT("LineTraceEndPoint"));

	LineTraceStartPoint->SetupAttachment(RootComponent);
	LineTraceEndPoint->SetupAttachment(RootComponent);

	AttackCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("AttackCollider"));
	AttackCollider->SetupAttachment(RootComponent);
	
	AttackCollider->OnComponentBeginOverlap.AddDynamic(this, &AFinalProjectAlphaCharacter::AttackOverlap);
	AttackCollider->OnComponentEndOverlap.AddDynamic(this, &AFinalProjectAlphaCharacter::AttackOverlapEnd);

	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AFinalProjectAlphaCharacter::OnOverlapBegin);
}

//////////////////////////////////////////////////////////////////////////
// Input

void AFinalProjectAlphaCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AFinalProjectAlphaCharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAxis("MoveForward", this, &AFinalProjectAlphaCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AFinalProjectAlphaCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AFinalProjectAlphaCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AFinalProjectAlphaCharacter::LookUpAtRate);

	PlayerInputComponent->BindAction("SetSlowTrap", IE_Pressed, this, &AFinalProjectAlphaCharacter::SetSlowTrap);
	PlayerInputComponent->BindAction("SetStunTrap", IE_Pressed, this, &AFinalProjectAlphaCharacter::SetStunTrap);
	PlayerInputComponent->BindAction("SetDamageTrap", IE_Pressed, this, &AFinalProjectAlphaCharacter::SetDamageTrap);

	PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &AFinalProjectAlphaCharacter::Attack);

	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &AFinalProjectAlphaCharacter::Sprint);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &AFinalProjectAlphaCharacter::StopSprint);
}

void AFinalProjectAlphaCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void AFinalProjectAlphaCharacter::Tick(float DeltaTime)
{
	DrawDebugLine(GetWorld(), LineTraceStartPoint->GetComponentLocation(), LineTraceEndPoint->GetComponentLocation(), FColor::Red);
}

void AFinalProjectAlphaCharacter::ChangeSpeed(float Speed, float Duration)
{
	GetCharacterMovement()->MaxWalkSpeed = Speed;
	GetWorld()->GetTimerManager().SetTimer(TrapTimerHandle, this, &AFinalProjectAlphaCharacter::SpeedReset, Duration, false);
}

void AFinalProjectAlphaCharacter::SpeedReset()
{
	GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;
	GetCharacterMovement()->RotationRate.Yaw = MaxRotation;
}

void AFinalProjectAlphaCharacter::BlockRotation()
{
	GetCharacterMovement()->RotationRate.Yaw = 0.f;
}

void AFinalProjectAlphaCharacter::Attack()
{
	if (bBossInArea && bCanAttack)
	{
		UE_LOG(LogTemp, Warning, TEXT("Attack"))

		if (BossRef->BossStun) 
		{
			BossRef->Berserk();
		}

		BossRef->DamageCalculation();

		bCanAttack = false;

		GetWorld()->GetTimerManager().SetTimer(TrapTimerHandle, this, &AFinalProjectAlphaCharacter::CanAttack, 0.3f, false);
	}
}

void AFinalProjectAlphaCharacter::CanAttack()
{
	bCanAttack = true;
}

void AFinalProjectAlphaCharacter::OnOverlapBegin(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherActor->ActorHasTag("CommonMaterial"))
	{
		CommonMaterial += 5;
		UE_LOG(LogTemp, Warning, TEXT("Common material + 1"))
	}
}

void AFinalProjectAlphaCharacter::AttackOverlap(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherActor->ActorHasTag("Boss"))
	{
		BossRef = Cast<ABoss>(OtherActor);
		
		if (BossRef)
		{
			UE_LOG(LogTemp, Warning, TEXT("Attack player is possible"))

			bBossInArea = true;

		}
	}
}

void AFinalProjectAlphaCharacter::AttackOverlapEnd(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor->ActorHasTag("Boss"))
	{
		UE_LOG(LogTemp, Warning, TEXT("Attack player no possible"));

		bBossInArea = false;
	}
}

#pragma region Movement and Jump
void AFinalProjectAlphaCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AFinalProjectAlphaCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AFinalProjectAlphaCharacter::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void AFinalProjectAlphaCharacter::MoveRight(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

void AFinalProjectAlphaCharacter::Jump()
{
	if (Stamina > 4) {
		Super::Jump();
		Stamina -= StaminaJump;
	}
}
#pragma endregion

#pragma region Stamina Function

void AFinalProjectAlphaCharacter::Sprint()
{
	ChangeSpeed(MaxSprint, MaxRotation);
	GetWorld()->GetTimerManager().ClearTimer(TimerStaminaIncrement);
	GetWorldTimerManager().SetTimer(TimerStaminaDecrement, this, &AFinalProjectAlphaCharacter::DecrementStamina, true, DownloadStamina);
}

void AFinalProjectAlphaCharacter::StopSprint()
{
	ChangeSpeed(NormalSpeed, MaxRotation);
	GetWorld()->GetTimerManager().ClearTimer(TimerStaminaDecrement);
	GetWorldTimerManager().SetTimer(TimerStaminaIncrement, this, &AFinalProjectAlphaCharacter::IncrementStamina, true, ReloadStamina);
}

void AFinalProjectAlphaCharacter::DecrementStamina()
{
	if (Stamina < 1) {
		StopSprint();
	}
	else
	{
		Stamina--;
		UE_LOG(LogTemp, Warning, TEXT("Stamina is %d"), Stamina);
	}
}

void AFinalProjectAlphaCharacter::IncrementStamina()
{
	if (Stamina > 14) {
		GetWorld()->GetTimerManager().ClearTimer(TimerStaminaIncrement);
	}
	else
	{
		Stamina++;
		UE_LOG(LogTemp, Warning, TEXT("Stamina is %d"), Stamina);
	}
}

#pragma endregion

#pragma region Trap Manager

void AFinalProjectAlphaCharacter::PlaceTrap()
{
	TrapCrafting();

	if (CommonTrap >= 1)
	{
		FVector SpawnLocation = GetTrapSpawnLocation(ECollisionChannel::ECC_WorldStatic).ImpactPoint;
		CommonTrap--;

		if (TrapToSpawn && SpawnLocation != FVector::ZeroVector)
		{
			FRotator SpawnRotation = FRotator::ZeroRotator;
			GetWorld()->SpawnActor(TrapToSpawn, &SpawnLocation, &SpawnRotation);

			UE_LOG(LogTemp, Warning, TEXT("%s spawned"), *TrapToSpawn->GetName())
		}
	}
}

void AFinalProjectAlphaCharacter::TrapCrafting()
{
	if (CommonMaterial >= 3)
	{
		CommonMaterial -= 3;
		CommonTrap++;

		UE_LOG(LogTemp, Warning, TEXT("Trap crafted: %s"), *TrapToSpawn->GetName())
			UE_LOG(LogTemp, Warning, TEXT("Materials amount: %i"), CommonMaterial)
			UE_LOG(LogTemp, Warning, TEXT("Traps amount: %i"), CommonTrap)
	}

	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Not enough materials"))
	}
}

FHitResult AFinalProjectAlphaCharacter::GetTrapSpawnLocation(ECollisionChannel CollisionChannel)
{
	FHitResult Hit;
	FCollisionQueryParams TraceParams(FName(TEXT("")), false, GetOwner());

	GetWorld()->LineTraceSingleByObjectType(Hit,
		LineTraceStartPoint->GetComponentLocation(),
		LineTraceEndPoint->GetComponentLocation(),
		FCollisionObjectQueryParams(CollisionChannel), TraceParams);

	return Hit;
}

void AFinalProjectAlphaCharacter::SetSlowTrap()
{
	TrapToSpawn = SlowTrap;
	PlaceTrap();
}

void AFinalProjectAlphaCharacter::SetStunTrap()
{
	TrapToSpawn = StunTrap;
	PlaceTrap();
}

void AFinalProjectAlphaCharacter::SetDamageTrap()
{
	TrapToSpawn = DamageTrap;
	PlaceTrap();
}

#pragma endregion