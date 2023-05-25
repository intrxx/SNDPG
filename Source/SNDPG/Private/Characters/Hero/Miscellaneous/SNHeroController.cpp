// Copyright 2023 Michal Oginski.


#include "Characters/Hero/Miscellaneous/SNHeroController.h"

#include "Characters/Hero/SNHero.h"
#include "Characters/Hero/Miscellaneous/SNBasicAttributesComponent.h"
#include "UI/SNHeroHUD.h"
#include "Characters/Hero/Miscellaneous/SNHeroState.h"
#include "GAS/SNAbilitySystemComponent.h"
#include "UI/SNFloatingDmgNumberWComponent.h"

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
	
	ASNHero* Hero = Cast<ASNHero>(GetCharacter());
	const USNBasicAttributesComponent* AC = USNBasicAttributesComponent::FindAttributeComponent(Hero);
	
	HeroHUD = CreateWidget<USNHeroHUD>(this, HeroHUDClass);
	HeroHUD->AddToViewport();

	HeroHUD->SetHealth(AC->GetHealth());
	HeroHUD->SetMaxHealth(AC->GetMaxHealth());
	HeroHUD->SetHealthPercentage(AC->GetHealth() / FMath::Max<float>(AC->GetMaxHealth(), 1.0f));

	HeroHUD->SetResource(AC->GetResource());
	HeroHUD->SetMaxResource(AC->GetMaxResource());
	HeroHUD->SetResourcePercentage(AC->GetResource() / FMath::Max<float>(AC->GetMaxResource(), 1.0f));

	HeroHUD->SetCharacterLevel(AC->GetCharacterLevel());
	HeroHUD->SetExperience(AC->GetExperience());
	HeroHUD->SetMaxExperience(AC->GetMaxExperience());
	HeroHUD->SetArmour(AC->GetArmour());
	HeroHUD->SetStrength(AC->GetStrength());
	HeroHUD->SetEndurance(AC->GetEndurance());
	HeroHUD->SetFaith(AC->GetFaith());
	HeroHUD->SetHealingRange(AC->GetHealing(), AC->GetFaith());
	HeroHUD->SetVitality(AC->GetVitality());
}

USNHeroHUD* ASNHeroController::GetHeroHUD()
{
	return HeroHUD;
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
	Super::PostProcessInput(DeltaTime, bGamePaused);
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
