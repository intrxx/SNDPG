// Copyright 2023 Michal Oginski.

#pragma once

#include "CoreMinimal.h"
#include "GAS/SNGameplayAbility.h"
#include "SNGameplayAbility_Melee.generated.h"

class USNCombatComponent;
/**
 * 
 */
UCLASS()
class SNDPG_API USNGameplayAbility_Melee : public USNGameplayAbility
{
	GENERATED_BODY()

public:
	USNGameplayAbility_Melee(const FObjectInitializer& ObjectInitializer);
	
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	virtual void PreActivate(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, FOnGameplayAbilityEnded::FDelegate* OnGameplayAbilityEndedDelegate, const FGameplayEventData* TriggerEventData) override;
};
