// Copyright 2023 Michal Oginski.


#include "GAS/Abilities/SNGameplayAbility_Death.h"
#include "GAS/SNAbilitySystemComponent.h"
#include "GameplayTagsManager.h"
#include "ActorComponents/SNBasicAttributesComponent.h"
#include "GameplayTags/SNGameplayTags.h"

USNGameplayAbility_Death::USNGameplayAbility_Death(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::ServerInitiated;

	bAutoStartDeath = true;

	UGameplayTagsManager::Get().CallOrRegister_OnDoneAddingNativeTagsDelegate(FSimpleDelegate::CreateUObject(this, &ThisClass::DoneAddingNativeTags));
}

void USNGameplayAbility_Death::DoneAddingNativeTags()
{
	if (HasAnyFlags(RF_ClassDefaultObject))
	{
		// Add the ability trigger tag as default to the CDO.
		FAbilityTriggerData TriggerData;
		TriggerData.TriggerTag = FSNGameplayTags::Get().GameplayEvent_Death;
		TriggerData.TriggerSource = EGameplayAbilityTriggerSource::GameplayEvent;
		AbilityTriggers.Add(TriggerData);
	}
}

void USNGameplayAbility_Death::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	check(ActorInfo);

	USNAbilitySystemComponent* SNASC = CastChecked<USNAbilitySystemComponent>(ActorInfo->AbilitySystemComponent.Get());

	FGameplayTagContainer AbilityTypesToIgnore;
	AbilityTypesToIgnore.AddTag(FSNGameplayTags::Get().Ability_Behavior_SurvivesDeath);

	// Cancel all abilities and block others from starting.
	SNASC->CancelAbilities(nullptr, &AbilityTypesToIgnore, this);

	SetCanBeCanceled(false);
	
	if (bAutoStartDeath)
	{
		StartDeath();
	}

	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void USNGameplayAbility_Death::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	check(ActorInfo);

	// Always try to finish the death when the ability ends in case the ability doesn't.
	// This won't do anything if the death hasn't been started.
	FinishDeath();

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void USNGameplayAbility_Death::StartDeath()
{
	if (USNBasicAttributesComponent* HealthComponent = USNBasicAttributesComponent::FindAttributeComponent(GetAvatarActorFromActorInfo()))
	{
		if (HealthComponent->GetDeathState() == ESNDeathState::NotDead)
		{
			HealthComponent->StartDeath();
		}
	}
}

void USNGameplayAbility_Death::FinishDeath()
{
	if (USNBasicAttributesComponent* HealthComponent = USNBasicAttributesComponent::FindAttributeComponent(GetAvatarActorFromActorInfo()))
	{
		if (HealthComponent->GetDeathState() == ESNDeathState::DeathStarted)
		{
			HealthComponent->FinishDeath();
		}
	}
}
