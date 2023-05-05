// Copyright 2023 Michal Oginski.


#include "Characters/Hero/Miscellaneous/SNHeroController.h"

#include "Characters/Hero/Miscellaneous/SNHeroState.h"
#include "GAS/SNAbilitySystemComponent.h"

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
	return PS->GetSNAbilitySystemComponent();
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
