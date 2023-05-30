// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Abilities/SNGameplayAbility_Melee.h"

#include "Characters/Hero/SNHero.h"

USNGameplayAbility_Melee::USNGameplayAbility_Melee(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void USNGameplayAbility_Melee::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                               const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                               const FGameplayEventData* TriggerEventData)
{
	ASNHero* Hero = Cast<ASNHero>(GetSNHeroFromActorInfo());
	if(Hero)
	{
		//Hero->StartMeleeCombo();
	}
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void USNGameplayAbility_Melee::PreActivate(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	FOnGameplayAbilityEnded::FDelegate* OnGameplayAbilityEndedDelegate, const FGameplayEventData* TriggerEventData)
{
	
	Super::PreActivate(Handle, ActorInfo, ActivationInfo, OnGameplayAbilityEndedDelegate, TriggerEventData);
}
