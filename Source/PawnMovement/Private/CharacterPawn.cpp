// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterPawn.h"
#include "MyPlayerController.h"
#include "EnhancedInputComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
ACharacterPawn::ACharacterPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
	RootComponent = CapsuleComp;
	CapsuleComp->SetSimulatePhysics(false);
	
	SkeletalMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComponent"));
	SkeletalMeshComp->SetupAttachment(RootComponent);
	SkeletalMeshComp->SetSimulatePhysics(false);
	
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArmComp->SetupAttachment(CapsuleComp);
	SpringArmComp->TargetArmLength = 300.0f;
	SpringArmComp->bUsePawnControlRotation = true;
	
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComp->SetupAttachment(SpringArmComp, USpringArmComponent::SocketName);
	CameraComp->bUsePawnControlRotation = false;
}

// Called when the game starts or when spawned
void ACharacterPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACharacterPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ACharacterPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (AMyPlayerController* PlayerController = Cast<AMyPlayerController>(GetController()))
		{
			if (PlayerController->MoveAction)
			{
				EnhancedInput->BindAction(
					PlayerController->MoveAction,
					ETriggerEvent::Triggered,
					this,
					&ACharacterPawn::Move
					);
			}
			
			/*if (PlayerController->JumpAction)
			{
				EnhancedInput->BindAction(
					PlayerController->JumpAction,
					ETriggerEvent::Triggered,
					this,
					&ACharacterPawn::StartJump
					);
				
				EnhancedInput->BindAction(
					PlayerController->JumpAction,
					ETriggerEvent::Completed,
					this,
					&ACharacterPawn::StopJump
					);
			}*/
			
			if (PlayerController->LookAction)
			{
				
				EnhancedInput->BindAction(
					PlayerController->LookAction,
					ETriggerEvent::Triggered,
					this,
					&ACharacterPawn::Look
					);
			}
			
			/*if (PlayerController->SprintAction)
			{
				EnhancedInput->BindAction(
					PlayerController->SprintAction,
					ETriggerEvent::Triggered,
					this,
					&ACharacterPawn::StartSprint
					);
				
				EnhancedInput->BindAction(
					PlayerController->SprintAction,
					ETriggerEvent::Completed,
					this,
					&ACharacterPawn::StopSprint
					);
			}*/
		}
	}
}

void ACharacterPawn::Move(const FInputActionValue& value)
{
	if (!Controller) return;
	
	const FVector2D MoveInput = value.Get<FVector2D>();
	
	if (!FMath::IsNearlyZero(MoveInput.X))
	{
		AddActorLocalOffset(GetActorForwardVector() * MoveInput.X * MoveSpeed * GetWorld()->GetDeltaSeconds());
	}
	
	if (!FMath::IsNearlyZero(MoveInput.Y))
	{
		AddActorLocalOffset(GetActorRightVector() * MoveInput.Y * MoveSpeed * GetWorld()->GetDeltaSeconds());
	}
}

/*void ACharacterPawn::StartJump(const FInputActionValue& value)
{
	if (!Controller) return;
	
	if (value.Get<bool>())
	{
		Jump();
	}
}*/

/*void ACharacterPawn::StopJump(const FInputActionValue& value)
{
	if (!Controller) return;
	
	if (!value.Get<bool>())
	{
		StopJumping();
	}
}*/
void ACharacterPawn::Look(const FInputActionValue& value)
{
	UE_LOG(LogTemp, Warning, TEXT("ActorRotation: %s"), *GetActorRotation().ToString());
	if (!Controller) return;
	
	FVector2D LookInput = value.Get<FVector2D>();
	
	if (!FMath::IsNearlyZero(LookInput.X))
	{
		FRotator YawRotation(0.f, GetActorForwardVector().X * LookSpeed * GetWorld()->GetDeltaSeconds(), 0.f);
		AddActorLocalRotation(YawRotation);
	}
	
	if (!FMath::IsNearlyZero(LookInput.Y))
	{
		FRotator PitchRotation(GetActorRightVector().Y * LookSpeed * GetWorld()->GetDeltaSeconds(), 0.f, 0.f);
		SpringArmComp->AddLocalRotation(PitchRotation);
	}
}

/*void ACharacterPawn::StartSprint(const FInputActionValue& value)
{
	if (!Controller) return;
	
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
	}
}

void ACharacterPawn::StopSprint(const FInputActionValue& value)
{
	if (!Controller) return;
	
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;
	}
}*/

