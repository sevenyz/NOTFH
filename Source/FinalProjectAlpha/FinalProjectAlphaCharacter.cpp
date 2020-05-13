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
#include "NewMinion.h"
#include "Player/MyPlayerController.h"
#include "GameFramework/WorldSettings.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"

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

	AttackCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("AttackCollider"));
	AttackCollider->SetupAttachment(RootComponent);
	
	AttackCollider->OnComponentBeginOverlap.AddDynamic(this, &AFinalProjectAlphaCharacter::AttackOverlap);
	AttackCollider->OnComponentEndOverlap.AddDynamic(this, &AFinalProjectAlphaCharacter::AttackOverlapEnd);

	CraftingMode = false;

}

//////////////////////////////////////////////////////////////////////////
// Input

void AFinalProjectAlphaCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAxis("MoveForward", this, &AFinalProjectAlphaCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AFinalProjectAlphaCharacter::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AFinalProjectAlphaCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AFinalProjectAlphaCharacter::LookUpAtRate);

	PlayerInputComponent->BindAction("ScrollUpTrap", IE_Pressed, this, &AFinalProjectAlphaCharacter::ScrollUp);
	PlayerInputComponent->BindAction("ScrollDownTrap", IE_Pressed, this, &AFinalProjectAlphaCharacter::ScrollDown);

	PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &AFinalProjectAlphaCharacter::Attack);

	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &AFinalProjectAlphaCharacter::Sprint);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &AFinalProjectAlphaCharacter::StopSprint);

	PlayerInputComponent->BindAction("Crafting", IE_Pressed, this, &AFinalProjectAlphaCharacter::OpenPannelCrafting);
	PlayerInputComponent->BindAction("Crafting", IE_Released, this, &AFinalProjectAlphaCharacter::ClosePannelCrafting);
}

void AFinalProjectAlphaCharacter::BeginPlay()
{
	Super::BeginPlay();

	PlayerControllerRef = Cast<AMyPlayerController>(UGameplayStatics::GetPlayerController(this, 0));

	delegateMaster = Cast<ADelegateMaster>(UGameplayStatics::GetActorOfClass(GetWorld(), ADelegateMaster::StaticClass()));

}


void AFinalProjectAlphaCharacter::Tick(float DeltaTime)
{
	//DrawDebugLine(GetWorld(), LineTraceStartPoint->GetComponentLocation(), LineTraceEndPoint->GetComponentLocation(), FColor::Red);
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

// forse da levare
void AFinalProjectAlphaCharacter::BlockRotation()
{
	GetCharacterMovement()->RotationRate.Yaw = 0.f;
}

void AFinalProjectAlphaCharacter::Attack()
{
	if (bCanAttack)
	{
		if (bBossInArea)
		{

			if (BossRef->bBossStun)
			{
				BossRef->Berserk();
			}

			BossRef->DamageCalculation();

			bCanAttack = false;

			// implementare con animazione
			GetWorld()->GetTimerManager().SetTimer(TrapTimerHandle, this, &AFinalProjectAlphaCharacter::CanAttack, 0.5f, false);

		}

		if (bMinionArea)
		{

			minionRef->CalculateDamage(Damage);

			bCanAttack = false;

			// implementare con animazione
			GetWorld()->GetTimerManager().SetTimer(TrapTimerHandle, this, &AFinalProjectAlphaCharacter::CanAttack, 0.5f, false);

		}
	}
}

// da implementare con l'animazione
void AFinalProjectAlphaCharacter::CanAttack()
{
	bCanAttack = true;
}

void AFinalProjectAlphaCharacter::AttackOverlap(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherActor->ActorHasTag("Boss"))
	{
		BossRef = Cast<ABoss>(OtherActor);

		if (BossRef)
		{
			bBossInArea = true;
		}
	}

	if (OtherActor->ActorHasTag("Minion"))
	{
		minionRef = Cast<ANewMinion>(OtherActor);

		if (minionRef)
		{
			bMinionArea = true;
		}
	}
}

void AFinalProjectAlphaCharacter::AttackOverlapEnd(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor->ActorHasTag("Boss"))
	{
		UE_LOG(LogTemp, Warning, TEXT("Attack player no possible Boss"));

		bBossInArea = false;
	}

	else if (OtherActor->ActorHasTag("Minion"))
	{
		UE_LOG(LogTemp, Warning, TEXT("Attack player no possible Minion"));

		bMinionArea = false;
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
	}
}

#pragma endregion

void AFinalProjectAlphaCharacter::OpenPannelCrafting()
{
	if (PlayerControllerRef != nullptr)
	{
		PlayerControllerRef->OpenCrafting();
		CameraBoom->bUsePawnControlRotation = false;
		GetCharacterMovement()->bOrientRotationToMovement = false;
		UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 0.2);
		this->CustomTimeDilation = 1;
	}
}

void AFinalProjectAlphaCharacter::ClosePannelCrafting()
{
	if (PlayerControllerRef != nullptr)
	{
		PlayerControllerRef->CloseCrafting();
		CameraBoom->bUsePawnControlRotation = true;
		GetCharacterMovement()->bOrientRotationToMovement = true;
		delegateMaster->SwitchImage.ExecuteIfBound();
		UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1);
		this->CustomTimeDilation = 1;
	}
}

void AFinalProjectAlphaCharacter::ScrollUp()
{
	delegateMaster->bImageUp = true;
	delegateMaster->SwitchImage.ExecuteIfBound();
}

void AFinalProjectAlphaCharacter::ScrollDown()
{
	delegateMaster->bImageDown = true;
	delegateMaster->SwitchImage.ExecuteIfBound();
}
