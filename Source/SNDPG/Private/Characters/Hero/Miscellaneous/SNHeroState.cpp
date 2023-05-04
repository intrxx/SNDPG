// Copyright 2023 Michal Oginski.


#include "Characters/Hero/Miscellaneous/SNHeroState.h"
#include "GAS/SNAbilitySystemComponent.h"


ASNHeroState::ASNHeroState()
{
	AbilitySystemComponent = CreateDefaultSubobject<USNAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);

	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Full);
	
	NetUpdateFrequency = 100.0f;
}

UAbilitySystemComponent* ASNHeroState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}
