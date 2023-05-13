// Copyright 2023 Michal Oginski.


#include "Characters/Hero/SNHero.h"
#include "GAS/SNAbilitySystemComponent.h"
#include "Characters/SNCharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Characters/Hero/Miscellaneous/SNBasicAttributesComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Characters/Hero/Miscellaneous/SNHeroController.h"
#include "Characters/Hero/Miscellaneous/SNHeroState.h"
#include "Components/CapsuleComponent.h"
#include "GameplayTags/SNGameplayTags.h"
#include "GAS/SNAbilitySet.h"
#include "GAS/SNGameplayAbility.h"
#include "Input/SNEnhancedInputComponent.h"
#include "UI/SNHeroHUD.h"

ASNHero::ASNHero(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;
	
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	USNCharacterMovementComponent* SNMoveComp = CastChecked<USNCharacterMovementComponent>(GetCharacterMovement());
	SNMoveComp->bOrientRotationToMovement = true;
	SNMoveComp->RotationRate = FRotator(0.0f, 500.0f,0.0f);
	
	UCapsuleComponent* CapsuleComp = GetCapsuleComponent();
	check(CapsuleComp);
	CapsuleComp->InitCapsuleSize(44.0f, 140.0f);

	USkeletalMeshComponent* MeshComp = GetMesh();
	check(MeshComp);
	MeshComp->SetRelativeRotation(FRotator(0.0f, -90.0f,0.0f));
	MeshComp->SetRelativeLocation(FVector(0.0f,0.0f,-110.0f));

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArmComp->SetupAttachment(RootComponent);
	SpringArmComp->TargetArmLength = 600;
	SpringArmComp->bUsePawnControlRotation = true;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(SpringArmComp, USpringArmComponent::SocketName);
	CameraComponent->bUsePawnControlRotation = false;

	AttributesComponent = CreateDefaultSubobject<USNBasicAttributesComponent>(TEXT("BasicAttributesComponent"));
}

void ASNHero::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	
	ASNHeroState* PS = Cast<ASNHeroState>(GetPlayerState());
	check(PS);

	AbilitySystemComponent = Cast<USNAbilitySystemComponent>(PS->GetAbilitySystemComponent());
	PS->GetAbilitySystemComponent()->InitAbilityActorInfo(PS,this);
	
	//TODO For now initialize here, later on I can make delegate when whole initialization process starts and then bind it to that delegate in this class's constructor
	
	AbilitySet->GiveToAbilitySystem(AbilitySystemComponent.Get(), nullptr, this);
	AttributesComponent->InitializeWithAbilitySystem(AbilitySystemComponent.Get());

	ASNHeroController* PC = Cast<ASNHeroController>(GetController());
	PC->CreateHeroHUD();
	
	//FOR DEBUG
	/*
	TArray<FGameplayAbilitySpec> ActivableAbilities;
	ActivableAbilities = AbilitySystemComponent->GetActivatableAbilities();
	for(FGameplayAbilitySpec Abilities : ActivableAbilities)
	{
		FString AbilityName = Abilities.Ability->GetName();
		UE_LOG(LogTemp, Warning, TEXT("Active ability: %s"), *AbilityName);
	}
	*/
}

void ASNHero::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	USNEnhancedInputComponent* EInputComponent = Cast<USNEnhancedInputComponent>(PlayerInputComponent);
	check(EInputComponent);

	const FSNGameplayTags& GameplayTags = FSNGameplayTags::Get();
	TArray<uint32> BindHandles;
	EInputComponent->BindAbilityActions(InputConfig, this, &ThisClass::InputAbilityInputTagPressed,
		&ThisClass::InputAbilityInputTagReleased, /*out*/ BindHandles);
	EInputComponent->BindNativeAction(InputConfig, GameplayTags.Input_Move, ETriggerEvent::Triggered, this,
		&ThisClass::Move);
	EInputComponent->BindNativeAction(InputConfig, GameplayTags.Input_Look, ETriggerEvent::Triggered, this,
		&ThisClass::Look);
	EInputComponent->BindNativeAction(InputConfig, GameplayTags.Input_ToggleCharacterStatus, ETriggerEvent::Triggered,
		this, &ThisClass::ToggleCharacterStatus);
}

void ASNHero::BeginPlay()
{
	Super::BeginPlay();

	if(ASNHeroController* PC = Cast<ASNHeroController>(GetController()))
	{
		if(UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	
}

void ASNHero::InputAbilityInputTagPressed(FGameplayTag InputTag)
{
	AbilitySystemComponent->AbilityInputTagPressed(InputTag);
}

void ASNHero::InputAbilityInputTagReleased(FGameplayTag InputTag)
{
	AbilitySystemComponent->AbilityInputTagReleased(InputTag);
}

void ASNHero::Move(const FInputActionValue& Value)
{
	const FVector2D DirectionValue = Value.Get<FVector2D>();
	if(GetController())
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0.0f, Rotation.Yaw, 0.0f);

		const FVector ForwardVector = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(ForwardVector, DirectionValue.Y);

		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(RightDirection, DirectionValue.X);
	}
}

void ASNHero::Look(const FInputActionValue& Value)
{
	const FVector2D LookValue = Value.Get<FVector2D>();

	if(GetController())
	{
		AddControllerYawInput(LookValue.X);
		AddControllerPitchInput(-LookValue.Y);
	}
}

void ASNHero::ToggleCharacterStatus()
{
	UE_LOG(LogTemp, Warning, TEXT("toggling"));
	ASNHeroController* PC = Cast<ASNHeroController>(GetController());
	if(PC)
	{
		USNHeroHUD* HUD = PC->GetHeroHUD();
		if(HUD)
		{
			if(HUD->bIsCharacterStatusVisible)
			{
				HUD->ToggleCharacterStatusEvent(HUD->bIsCharacterStatusVisible = false);
			}
			else
			{
				PC->GetHeroHUD()->ToggleCharacterStatusEvent(HUD->bIsCharacterStatusVisible = true);
			}
		}
	}
}



