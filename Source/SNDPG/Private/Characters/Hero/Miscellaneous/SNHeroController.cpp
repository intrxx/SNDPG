// Copyright 2023 Michał Ogiński.


#include "Characters/Hero/Miscellaneous/SNHeroController.h"
#include "UI/Inventory/SNInventoryWidget.h"
#include "Characters/Hero/SNHero.h"
#include "ActorComponents/SNBasicAttributesComponent.h"
#include "InventorySystem/SNEquipmentComponent.h"
#include "UI/HeroHUD/SNHeroHUD.h"
#include "UI/Menus/SNInGameMenu.h"
#include "UI/Inventory/SNEquipmentWidget.h"
#include "UI/HeroHUD/SNCharacterStatusWidget.h"
#include "Characters/Hero/Miscellaneous/SNHeroState.h"
#include "GAS/SNAbilitySystemComponent.h"
#include "UI/Combat/SNFloatingDmgNumberWComponent.h"

ASNHeroController::ASNHeroController()
{
}

ASNHeroState* ASNHeroController::GetSNPlayerState() const
{
	return CastChecked<ASNHeroState>(PlayerState, ECastCheckedType::NullAllowed);
}

USNAbilitySystemComponent* ASNHeroController::GetSNAbilitySystemComponent() const
{
	const ASNHeroState* PS = GetSNPlayerState();
	return CastChecked<USNAbilitySystemComponent>(PS->GetAbilitySystemComponent());
}

void ASNHeroController::CreateHeroHUD()
{
	if(HeroHUD)
	{
		return;
	}

	if(!HeroHUDClass)
	{
		UE_LOG(LogTemp, Error, TEXT("%s() Missing MainCharacterHUDClass. Fill in on the BP in PlayerController."), *FString(__FUNCTION__));
		return;
	}
	
	HeroHUD = CreateWidget<USNHeroHUD>(this, HeroHUDClass);
	HeroHUD->AddToViewport();
	
	ASNHero* Hero = Cast<ASNHero>(GetCharacter());
	check(Hero);
	
	const USNBasicAttributesComponent* AttributesComp = USNBasicAttributesComponent::FindAttributeComponent(Hero);
	check(AttributesComp);
	
	HeroHUD->SetHealthPercentage(AttributesComp->GetHealth() / FMath::Max<float>(AttributesComp->GetMaxHealth(), 1.0f));
	HeroHUD->SetMaxHealth(AttributesComp->GetMaxHealth());
	
	HeroHUD->SetResourcePercentage(AttributesComp->GetResource() / FMath::Max<float>(AttributesComp->GetMaxResource(), 1.0f));
	HeroHUD->SetMaxResource(AttributesComp->GetMaxResource());
	
	HeroHUD->SetStaminaPercentage(AttributesComp->GetStamina() / FMath::Max<float>(AttributesComp->GetMaxStamina(), 1.0f));
	HeroHUD->SetMaxStamina(AttributesComp->GetMaxStamina());

	HeroHUD->SetGold(AttributesComp->GetGold());
	
	CreateCharacterStatusUI(AttributesComp);
	CreateInventoryUI(AttributesComp);
	CreateEquipmentUI(AttributesComp);
	CreateInGameMenuUI();
}

void ASNHeroController::CreateInventoryUI(const USNBasicAttributesComponent* AttributesComp)
{
	if(InventoryWidget)
	{
		return;
	}

	if(!InventoryWidgetClass)
	{
		UE_LOG(LogTemp, Error, TEXT("%s() Missing InventoryWidgetClass. Fill in on the BP in PlayerController."), *FString(__FUNCTION__));
		return;
	}
	
	InventoryWidget = CreateWidget<USNInventoryWidget>(this, InventoryWidgetClass);
	InventoryWidget->AddToViewport();
	InventoryWidget->SetVisibility(ESlateVisibility::Collapsed);

	InventoryWidget->SetHealth(AttributesComp->GetHealth(), AttributesComp->GetMaxHealth());
	InventoryWidget->SetResource(AttributesComp->GetResource(), AttributesComp->GetMaxResource());
	InventoryWidget->SetMaxStamina(AttributesComp->GetMaxStamina());
	
	InventoryWidget->SetCharacterLevel(AttributesComp->GetCharacterLevel());
	InventoryWidget->SetExperience(AttributesComp->GetExperience());
	InventoryWidget->SetVitality(AttributesComp->GetVitality());
	InventoryWidget->SetMind(AttributesComp->GetMind());
	InventoryWidget->SetEndurance(AttributesComp->GetEndurance());
	InventoryWidget->SetStrength(AttributesComp->GetStrength());
	InventoryWidget->SetFaith(AttributesComp->GetFaith());
	InventoryWidget->SetArcane(AttributesComp->GetArcane());

	InventoryWidget->SetArmour(AttributesComp->GetArmour());
	InventoryWidget->SetGold(AttributesComp->GetGold());
}

void ASNHeroController::CreateInGameMenuUI()
{
	if(InGameMenuWidget)
	{
		return;
	}

	if(!InGameMenuWidgetClass)
	{
		UE_LOG(LogTemp, Error, TEXT("%s() Missing InGameMenuClass. Fill in on the BP in PlayerController."), *FString(__FUNCTION__));
		return;
	}

	InGameMenuWidget = CreateWidget<USNInGameMenu>(this, InGameMenuWidgetClass);
	InGameMenuWidget->AddToViewport();
	InGameMenuWidget->SetVisibility(ESlateVisibility::Collapsed);
}

void ASNHeroController::CreateCharacterStatusUI(const USNBasicAttributesComponent* AttributesComp)
{
	if(CharacterStatusWidget)
	{
		return;
	}

	if(!CharacterStatusWidgetClass)
	{
		UE_LOG(LogTemp, Error, TEXT("%s() Missing InGameMenuClass. Fill in on the BP in PlayerController."), *FString(__FUNCTION__));
		return;
	}
	
	ASNHero* Hero = Cast<ASNHero>(GetCharacter());
	check(Hero);
	
	CharacterStatusWidget = CreateWidget<USNCharacterStatusWidget>(this, CharacterStatusWidgetClass);
	CharacterStatusWidget->AddToViewport();
	CharacterStatusWidget->SetVisibility(ESlateVisibility::Collapsed);
	
	CharacterStatusWidget->SetHealth(AttributesComp->GetHealth(), AttributesComp->GetMaxHealth());
	CharacterStatusWidget->SetMaxHealth(AttributesComp->GetMaxHealth());
	
	CharacterStatusWidget->SetResource(AttributesComp->GetResource(), AttributesComp->GetMaxResource());
	CharacterStatusWidget->SetMaxResource(AttributesComp->GetMaxResource());
	
	CharacterStatusWidget->SetMaxStamina(AttributesComp->GetMaxStamina());
	
	CharacterStatusWidget->SetCharacterLevel(AttributesComp->GetCharacterLevel());
	CharacterStatusWidget->SetLevelUpPoints(AttributesComp->GetLevelUpPoints());
	CharacterStatusWidget->SetExperience(AttributesComp->GetExperience());
	CharacterStatusWidget->SetMaxExperience(AttributesComp->GetMaxExperience());
	CharacterStatusWidget->SetArmour(AttributesComp->GetArmour());
	CharacterStatusWidget->SetStrength(AttributesComp->GetStrength());
	CharacterStatusWidget->SetEndurance(AttributesComp->GetEndurance());
	CharacterStatusWidget->SetFaith(AttributesComp->GetFaith());
	CharacterStatusWidget->SetVitality(AttributesComp->GetVitality());
	CharacterStatusWidget->SetArcane(AttributesComp->GetArcane());
	CharacterStatusWidget->SetMind(AttributesComp->GetMind());

	const USNEquipmentComponent* Equipment = USNEquipmentComponent::FindEquipmentComponent(Hero);
	check(Equipment);
	
	// Set Damage/Healing ranges
	CharacterStatusWidget->SetHealingRange(AttributesComp->GetHealing(), AttributesComp->GetFaith());
	CharacterStatusWidget->SetReplenishingRange(AttributesComp->GetReplenishing(), AttributesComp->GetFaith());
	
	CharacterStatusWidget->SetLightAttackRange(Equipment->GetEquippedLightAttackWeaponDamage(), AttributesComp->GetStrength());
	CharacterStatusWidget->SetHeavyAttackRange(Equipment->GetEquippedHeavyAttackWeaponDamage(), AttributesComp->GetStrength());
	CharacterStatusWidget->SetItemDamageRange(Equipment->GetEquippedThrowingWeaponDamage(), AttributesComp->GetStrength());
	CharacterStatusWidget->SetWeaponSpellDamage(Equipment->GetEquippedWeaponSpellDamage(), AttributesComp->GetArcane());
}

void ASNHeroController::CreateEquipmentUI(const USNBasicAttributesComponent* AttributesComp)
{
	if(EquipmentWidget)
	{
		return;
	}

	if(!EquipmentWidgetClass)
	{
		UE_LOG(LogTemp, Error, TEXT("%s() Missing InGameMenuClass. Fill in on the BP in PlayerController."), *FString(__FUNCTION__));
		return;
	}

	EquipmentWidget = CreateWidget<USNEquipmentWidget>(this, EquipmentWidgetClass);
	EquipmentWidget->AddToViewport();
	EquipmentWidget->SetVisibility(ESlateVisibility::Collapsed);

	EquipmentWidget->SetHealth(AttributesComp->GetHealth(), AttributesComp->GetMaxHealth());
	EquipmentWidget->SetResource(AttributesComp->GetResource(), AttributesComp->GetMaxResource());
	EquipmentWidget->SetMaxStamina(AttributesComp->GetMaxStamina());
	
	EquipmentWidget->SetCharacterLevel(AttributesComp->GetCharacterLevel());
	EquipmentWidget->SetExperience(AttributesComp->GetExperience());
	EquipmentWidget->SetVitality(AttributesComp->GetVitality());
	EquipmentWidget->SetMind(AttributesComp->GetMind());
	EquipmentWidget->SetEndurance(AttributesComp->GetEndurance());
	EquipmentWidget->SetStrength(AttributesComp->GetStrength());
	EquipmentWidget->SetFaith(AttributesComp->GetFaith());
	EquipmentWidget->SetArcane(AttributesComp->GetArcane());

	EquipmentWidget->SetArmour(AttributesComp->GetArmour());
	EquipmentWidget->SetGold(AttributesComp->GetGold());
}

void ASNHeroController::ShowFloatingNumber(float Amount, ASNCharacterBase* TargetCharacter)
{
	if(TargetCharacter)
	{
		USNFloatingDmgNumberWComponent* FloatingText = NewObject<USNFloatingDmgNumberWComponent>(TargetCharacter, DamageNumberClass);
		FloatingText->RegisterComponent();
		FloatingText->AttachToComponent(TargetCharacter->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
		FloatingText->SetDamageText(Amount);
	}
}

void ASNHeroController::PreProcessInput(const float DeltaTime, const bool bGamePaused)
{
	Super::PreProcessInput(DeltaTime, bGamePaused);
}

void ASNHeroController::PostProcessInput(const float DeltaTime, const bool bGamePaused)
{
	if(USNAbilitySystemComponent* SNASC = GetSNAbilitySystemComponent())
	{
		SNASC->ProcessAbilityInput(DeltaTime, bGamePaused);
	}

	Super::PostProcessInput(DeltaTime, bGamePaused);
}

void ASNHeroController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	GetSNAbilitySystemComponent()->InitAbilityActorInfo(GetSNPlayerState(), InPawn);
}
