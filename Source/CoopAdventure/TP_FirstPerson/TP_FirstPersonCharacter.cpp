// Copyright Epic Games, Inc. All Rights Reserved.

#include "TP_FirstPersonCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "PlayerWidget.h"
#include "CoopAdventure/Components/InteractionComponent.h"

#define TRACE_INTERACTIVE ECC_GameTraceChannel1

//////////////////////////////////////////////////////////////////////////
// ATP_FirstPersonCharacter

ATP_FirstPersonCharacter::ATP_FirstPersonCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);
		
	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-10.f, 0.f, 60.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	//Mesh1P->SetRelativeRotation(FRotator(0.9f, -19.19f, 5.2f));
	Mesh1P->SetRelativeLocation(FVector(-30.f, 0.f, -150.f));

	TimeBetweenInteractions = 0.1f;
	TimeSinceLastInteract = 0.f;
}

void ATP_FirstPersonCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}

		if (WidgetToDisplay)
		{
			PlayerWidgetRef = CreateWidget<UPlayerWidget>(PlayerController, WidgetToDisplay);
			PlayerWidgetRef->AddToViewport();
		}
	}

}

void ATP_FirstPersonCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	TimeSinceLastInteract += DeltaSeconds;
	if (TimeSinceLastInteract > TimeBetweenInteractions)
	{
		TimeSinceLastInteract = 0.f;
		TryToInteract();
	}
}

//////////////////////////////////////////////////////////////////////////// Input

void ATP_FirstPersonCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		//Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		//Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ATP_FirstPersonCharacter::Move);

		//Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ATP_FirstPersonCharacter::Look);
	}
}

void ATP_FirstPersonCharacter::TryToInteract()
{
	FHitResult HitResult;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);

	FVector StartLoc;
	FRotator StartRot;
	GetActorEyesViewPoint(StartLoc, StartRot);
	const FVector EndLoc = (StartRot.Vector() * 300.f) + StartLoc;

	GetWorld()->LineTraceSingleByChannel(HitResult, StartLoc, EndLoc, TRACE_INTERACTIVE, QueryParams);

	if (HitResult.IsValidBlockingHit())
	{
		if (ActorBeingViewed != HitResult.GetActor() && PlayerWidgetRef)
		{
			ActorBeingViewed = HitResult.GetActor();
			
			if (UInteractionComponent* InteractionComponent = Cast<UInteractionComponent>(HitResult.GetActor()->GetComponentByClass(UInteractionComponent::StaticClass())))
			{
				PlayerWidgetRef->SetMessageText(InteractionComponent->GetTextToDisplay());
			}
		}
	}
	else
	{
		if (ActorBeingViewed != nullptr)
		{
			if (PlayerWidgetRef)
			{
				PlayerWidgetRef->SetMessageText(FText::FromString(""));
			}
			ActorBeingViewed = nullptr;
		}
	}
}


void ATP_FirstPersonCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add movement 
		AddMovementInput(GetActorForwardVector(), MovementVector.Y);
		AddMovementInput(GetActorRightVector(), MovementVector.X);
	}
}

void ATP_FirstPersonCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}