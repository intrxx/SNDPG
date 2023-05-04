// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Hero/Miscellaneous/SNHeroState.h"
#include "GAS/SNAbilitySystemComponent.h"


ASNHeroState::ASNHeroState()
{
	AbilitySystemComponent = CreateDefaultSubobject<USNAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);

	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Full);

	//AttributeSet = CreateDefaultSubobject<USNArenaAttributeSet>(TEXT("AttributeSet"));

	NetUpdateFrequency = 100.0f;
}

UAbilitySystemComponent* ASNHeroState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

/*
USNArenaAttributeSet* ASNHeroState::GetAttributeSet() const
{
	return AttributeSet;
}

float ASNHeroState::GetHealth() const
{
	return AttributeSet->GetHealth();
}
*/