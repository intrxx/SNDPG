// Copyright 2023 Michał Ogiński.


#include "Characters/Hero/SNHero.h"
#include "GAS/SNAbilitySystemComponent.h"
#include "Characters/SNCharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputSubsystems.h"
#include "ActorComponents/SNBasicAttributesComponent.h"
#include "ActorComponents/SNCombatComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "UI/Menus/SNInGameMenu.h"
#include "Characters/Hero/Miscellaneous/SNHeroController.h"
#include "Characters/Hero/Miscellaneous/SNHeroState.h"
#include "Components/CapsuleComponent.h"
#include "GameplayTags/SNGameplayTags.h"
#include "GAS/SNAbilitySet.h"
#include "Input/SNEnhancedInputComponent.h"
#include "InventorySystem/SNEquipmentComponent.h"
#include "InventorySystem/SNInventoryComponent.h"
#include "InventorySystem/SNItemBase.h"
#include "UI/HeroHUD/SNHeroHUD.h"

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
	CapsuleComp->InitCapsuleSize(38.0f, 84.0f);

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

	CombatComponent = CreateDefaultSubobject<USNCombatComponent>(TEXT("CombatComponent"));
	CombatComponent->AddTraceMesh(MeshComp);
	CombatComponent->IgnoredActors.Add(this);
	
	AttributesComponent = CreateDefaultSubobject<USNBasicAttributesComponent>(TEXT("BasicAttributesComponent"));
	AttributesComponent->OnDeathStarted.AddDynamic(this, &ThisClass::OnDeathStarted);
	AttributesComponent->OnDeathFinished.AddDynamic(this, &ThisClass::OnDeathFinished);

	InventoryComponent = CreateDefaultSubobject<USNInventoryComponent>(TEXT("InventoryComponent"));

	EquipmentComponent = CreateDefaultSubobject<USNEquipmentComponent>(TEXT("EquipmentComponent"));
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
	check(PC);
	
	PC->CreateHeroHUD();
}

void ASNHero::OnDeathStarted(AActor* OwningActor)
{
	DisableMovementAndCollision();
}

void ASNHero::OnDeathFinished(AActor* OwningActor)
{
	GetWorld()->GetTimerManager().SetTimerForNextTick(this, &ThisClass::DestroyDueToDeath);
}

void ASNHero::UseItem(USNItemBase* ItemToUse)
{
	if(ItemToUse)
	{
		ItemToUse->Use(this);
		ItemToUse->OnUse(this); //BP Event 
	}
}

void ASNHero::ApplyGameplayInputMappings()
{
	ASNHeroController* PC = Cast<ASNHeroController>(GetController());
	if(PC)
	{
		UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer());
		if(Subsystem)
		{
			Subsystem->ClearAllMappings();
			Subsystem->AddMappingContext(DefaultMappingContext_MNK, 0);
			Subsystem->AddMappingContext(DefaultMappingContext_Gamepad, 0);
		}
	}
}

void ASNHero::DestroyDueToDeath()
{
	Super::DestroyDueToDeath();

	AttributesComponent->UninitializeFromAbilitySystem();
}

void ASNHero::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	USNEnhancedInputComponent* EInputComponent = Cast<USNEnhancedInputComponent>(PlayerInputComponent);
	check(EInputComponent);
	
	const FSNGameplayTags& GameplayTags = FSNGameplayTags::Get();
	TArray<uint32> BindHandles;
	EInputComponent->BindAbilityActions(InputConfig, this, &ThisClass::InputAbilityInputTagPressed,
		&ThisClass::InputAbilityInputTagReleased, /*out*/ BindHandles);

	// Movement
	EInputComponent->BindNativeAction(InputConfig, GameplayTags.Input_Move, ETriggerEvent::Triggered, this,
		&ThisClass::Move);
	EInputComponent->BindNativeAction(InputConfig, GameplayTags.Input_Look_Mouse, ETriggerEvent::Triggered, this,
		&ThisClass::LookMouse);
	EInputComponent->BindNativeAction(InputConfig, GameplayTags.Input_Look_Stick, ETriggerEvent::Triggered, this,
		&ThisClass::LookStick);

	// HUD
	EInputComponent->BindNativeAction(InputConfig, GameplayTags.Input_ToggleInGameMenu, ETriggerEvent::Triggered,
		this, &ThisClass::ToggleInGameMenu);

	EInputComponent->BindNativeAction(InputConfig, GameplayTags.Input_ItemSwitch_WeaponLeftSlot, ETriggerEvent::Triggered,
		this, &ThisClass::SwitchItemLeft_Weapon);
	EInputComponent->BindNativeAction(InputConfig, GameplayTags.Input_ItemSwitch_WeaponRightSlot, ETriggerEvent::Triggered,
		this, &ThisClass::SwitchItemRight_Weapon);
	EInputComponent->BindNativeAction(InputConfig, GameplayTags.Input_ItemSwitch_ConsumableDownSlot, ETriggerEvent::Triggered,
		this, &ThisClass::SwitchItemDown_Consumable);
	EInputComponent->BindNativeAction(InputConfig, GameplayTags.Input_ItemSwitch_MagicUpSlot, ETriggerEvent::Triggered,
		this, &ThisClass::SwitchItemUp_Magic);
}

void ASNHero::BeginPlay()
{
	Super::BeginPlay();

	if(ASNHeroController* PC = Cast<ASNHeroController>(GetController()))
	{
		if(UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext_MNK, 0);
			Subsystem->AddMappingContext(DefaultMappingContext_Gamepad, 0);
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

void ASNHero::LookMouse(const FInputActionValue& Value)
{
	const FVector2D LookValue = Value.Get<FVector2D>();

	if(GetController())
	{
		if(LookValue.X != 0.0f)
		{
			AddControllerYawInput(LookValue.X);
		}

		if(LookValue.Y != 0.0f)
		{
			AddControllerPitchInput(-LookValue.Y);
		}
	}
}

void ASNHero::LookStick(const FInputActionValue& Value)
{
	const FVector2D LookValue = Value.Get<FVector2D>();

	const UWorld* World = GetWorld();
	check(World)

	if(GetController())
	{
		if (LookValue.X != 0.0f)
		{
			AddControllerYawInput(LookValue.X * Hero::LookYawRate * World->GetDeltaSeconds());
		}

		if (LookValue.Y != 0.0f)
		{
			AddControllerPitchInput(LookValue.Y * Hero::LookPitchRate * World->GetDeltaSeconds());
		}
	}
}

void ASNHero::SwitchItemRight_Weapon()
{
	USNEquipmentComponent* Equipment = USNEquipmentComponent::FindEquipmentComponent(this);
	if(Equipment && Equipment->CurrentlyEquippedRightHandWeapon)
	{
		Equipment->SwitchEquippedRightHandWeapon(Equipment->CurrentlyEquippedRightHandWeaponIndex);
		
		ASNHeroController* PC = Cast<ASNHeroController>(GetController());
		if(PC)
		{
			USNHeroHUD* HUD = PC->GetHeroHUD();
			if(HUD)
			{
				HUD->SetRightHandWeaponEquippedItemDisplay(Equipment->CurrentlyEquippedRightHandWeapon);
			}
		}	
	}
}

void ASNHero::SwitchItemLeft_Weapon()
{
	USNEquipmentComponent* Equipment = USNEquipmentComponent::FindEquipmentComponent(this);
	if(Equipment && Equipment->CurrentlyEquippedLeftHandWeapon)
	{
		Equipment->SwitchEquippedLeftHandWeapon(Equipment->CurrentlyEquippedLeftHandWeaponIndex);
		
		ASNHeroController* PC = Cast<ASNHeroController>(GetController());
		if(PC)
		{
			USNHeroHUD* HUD = PC->GetHeroHUD();
			if(HUD)
			{
				HUD->SetLeftHandWeaponEquippedItemDisplay(Equipment->CurrentlyEquippedLeftHandWeapon);
			}
		}	
	}
}

void ASNHero::SwitchItemDown_Consumable()
{
	USNEquipmentComponent* Equipment = USNEquipmentComponent::FindEquipmentComponent(this);
	if(Equipment && Equipment->CurrentlyEquippedConsumable)
	{
		Equipment->SwitchEquippedConsumable(Equipment->CurrentlyEquippedConsumableIndex);
		
		ASNHeroController* PC = Cast<ASNHeroController>(GetController());
		if(PC)
		{
			USNHeroHUD* HUD = PC->GetHeroHUD();
			if(HUD)
			{
				HUD->SetConsumableEquippedItemDisplay(Equipment->CurrentlyEquippedConsumable);
			}
		}	
	}
}

void ASNHero::SwitchItemUp_Magic()
{
	USNEquipmentComponent* Equipment = USNEquipmentComponent::FindEquipmentComponent(this);
	if(Equipment && Equipment->CurrentlyEquippedMagic)
	{
		Equipment->SwitchEquippedMagic(Equipment->CurrentlyEquippedMagicIndex);
		
		ASNHeroController* PC = Cast<ASNHeroController>(GetController());
		if(PC)
		{
			USNHeroHUD* HUD = PC->GetHeroHUD();
			if(HUD)
			{
				HUD->SetMagicEquippedItemDisplay(Equipment->CurrentlyEquippedMagic);
			}
		}	
	}
}

void ASNHero::ToggleInGameMenu()
{
	ASNHeroController* PC = Cast<ASNHeroController>(GetController());
	if(PC)
	{
		UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer());
		if(Subsystem)
		{
			USNInGameMenu* InGameMenu = PC->GetInGameMenuUI();
			if(InGameMenu)
			{
				InGameMenu->ToggleInGameMenu(InGameMenu->bIsInGameMenuVisible = true);
				Subsystem->ClearAllMappings();
				Subsystem->AddMappingContext(HUDMappingContext, 1);
			}
		}
	}
}




