#include "MyProjectCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "PlayerMappableInputConfig.h" // KEEP THIS !! 
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "MenuSettings/UI/Settings/Category/Bindings/Configuration/GameFeatureAction_AddInputBinding.h"
#include "MenuSettings/UI/Settings/Category/Bindings/Configuration/GameFeatureAction_AddInputContextMapping.h"
#include "..\UI\Settings\Category\Bindings\Configuration\CustomInputComponent.h"
#include "MenuSettings/UI/Settings/LocalSettings.h"
#include "UserSettings/EnhancedInputUserSettings.h"

//////////////////////////////////////////////////////////////////////////
// AMyProjectCharacter

AMyProjectCharacter::AMyProjectCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
}

void AMyProjectCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	if (InputComponent != nullptr)
	{
		InitializePlayerInput(InputComponent);
	}
}

void AMyProjectCharacter::InitializePlayerInput(UInputComponent* PlayerInputComponent)
{
	check(PlayerInputComponent);
	
	const APlayerController* PC = GetController<APlayerController>();
	check(PC);

	ULocalPlayer* LP = Cast<ULocalPlayer>(PC->GetLocalPlayer());
	check(LP);

	UEnhancedInputLocalPlayerSubsystem* Subsystem = LP->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	check(Subsystem);
	
	Subsystem->ClearAllMappings();
	
	for (const FInputMappingContextAndPriority& Mapping : DefaultInputMappings)
	{
		if (UInputMappingContext* IMC = Mapping.InputMapping.Get())
		{
			if (Mapping.bRegisterWithSettings)
			{
				if (UEnhancedInputUserSettings* Settings = Subsystem->GetUserSettings())
				{
					Settings->RegisterInputMappingContext(IMC);
				}
				
				FModifyContextOptions Options = {};
				Options.bIgnoreAllPressedKeysUntilRelease = false;
				// Actually add the config to the local player							
				Subsystem->AddMappingContext(IMC, Mapping.Priority, Options);
			}
		}
	}

	// The Custom Input Component has some additional functions to map Gameplay Tags to an Input Action.
	// If you want this functionality but still want to change your input component class, make it a subclass
	// of the UCustomInputComponent or modify this component accordingly.
	UCustomInputComponent* CustomIC = Cast<UCustomInputComponent>(PlayerInputComponent);
	if (ensureMsgf(CustomIC, TEXT("Unexpected Input Component class! The Gameplay Abilities will not be bound to their inputs. Change the input component to UCustomInputComponent or a subclass of it.")))
	{
		// Add the key mappings that may have been set by the player
		CustomIC->AddInputMappings(InputConfig, Subsystem);

		// This is where we actually bind and input action to a gameplay tag, which means that Gameplay Ability Blueprints will
		// be triggered directly by these input actions Triggered events. 
		TArray<uint32> BindHandles;
		CustomIC->BindAbilityActions(InputConfig, this, &ThisClass::Input_AbilityInputTagPressed, &ThisClass::Input_AbilityInputTagReleased, /*out*/ BindHandles);
	}

	ULocalSettings::Get()->RegisterInputMappingContextsForLocalPlayer( LP );
}

//////////////////////////////////////////////////////////////////////////
// Input

void AMyProjectCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		//Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		//Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMyProjectCharacter::Move);

		//Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AMyProjectCharacter::Look);

		//Escape
		EnhancedInputComponent->BindAction(EscapeAction, ETriggerEvent::Triggered, this, &AMyProjectCharacter::Escape);
		 
	}
}

void AMyProjectCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void AMyProjectCharacter::Look(const FInputActionValue& Value)
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

void AMyProjectCharacter::AddAdditionalInputConfig(const UCustomInputConfig* NewInputConfig)
{
	TArray<uint32> BindHandles;
	
	const APlayerController* PC = GetController<APlayerController>();
	check(PC);

	const ULocalPlayer* LP = PC->GetLocalPlayer();
	check(LP);

	UEnhancedInputLocalPlayerSubsystem* Subsystem = LP->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	check(Subsystem);

	UCustomInputComponent* CustomIC = Cast<UCustomInputComponent>(InputComponent);
	if (ensureMsgf(CustomIC, TEXT("Unexpected Input Component class! The Gameplay Abilities will not be bound to their inputs. Change the input component to UCustomInputComponent or a subclass of it.")))
	{
		CustomIC->BindAbilityActions(NewInputConfig, this, &ThisClass::Input_AbilityInputTagPressed, &ThisClass::Input_AbilityInputTagReleased, /*out*/ BindHandles);
	}
	
}

void AMyProjectCharacter::RemoveAdditionalInputConfig(const UCustomInputConfig* NewInputConfig)
{
}

void AMyProjectCharacter::Input_AbilityInputTagPressed(FGameplayTag InputTag)
{
	// if (const UCustomPawnExtensionComponent* PawnExtComp = UCustomPawnExtensionComponent::FindPawnExtensionComponent(Pawn))
	// {
	// 	if (UCustomAbilitySystemComponent* CustomASC = PawnExtComp->GetCustomAbilitySystemComponent())
	// 	{
	// 		CustomASC->AbilityInputTagPressed(InputTag);
	// 	}
	// }	
}

void AMyProjectCharacter::Input_AbilityInputTagReleased(FGameplayTag InputTag)
{
	// if (const UCustomPawnExtensionComponent* PawnExtComp = UCustomPawnExtensionComponent::FindPawnExtensionComponent(Pawn))
	// {
	// 	if (UCustomAbilitySystemComponent* CustomASC = PawnExtComp->GetCustomAbilitySystemComponent())
	// 	{
	// 		CustomASC->AbilityInputTagReleased(InputTag);
	// 	}
	// }
}

bool AMyProjectCharacter::IsReadyToBindInputs() const
{
	return bReadyToBindInputs;
}
