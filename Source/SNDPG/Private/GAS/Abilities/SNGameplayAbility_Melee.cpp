// Copyright 2023 Michal Oginski.


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
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void USNGameplayAbility_Melee::PreActivate(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	FOnGameplayAbilityEnded::FDelegate* OnGameplayAbilityEndedDelegate, const FGameplayEventData* TriggerEventData)
{
	
	Super::PreActivate(Handle, ActorInfo, ActivationInfo, OnGameplayAbilityEndedDelegate, TriggerEventData);
}
