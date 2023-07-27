// Copyright 2023 Michal Oginski.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "SNAbilitySystemComponent.generated.h"

/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FReceivedDamageDelegate, USNAbilitySystemComponent*, SourceASC, float, UnmitigatedDamage, float, MitigatedDamage);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FReceivedHealDelegate, USNAbilitySystemComponent*, SourceASC, float, HealingDone);

UCLASS()
class SNDPG_API USNAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()
public:
	USNAbilitySystemComponent();
	
	void AbilityInputTagPressed(const FGameplayTag& InputTag);
	void AbilityInputTagReleased(const FGameplayTag& InputTag);

	void ProcessAbilityInput(float DeltaTime, bool bGamePaused);
	void ClearAbilityInput();
	
	virtual void ReceivedDamage(USNAbilitySystemComponent* SourceASC, float UnmitigatedDamage, float MitigatedDamage);
	virtual void ReceivedHeal(USNAbilitySystemComponent* SourceASC, float HealingDone);
	
public:
	FReceivedDamageDelegate ReceivedDamageDelegate;
	FReceivedHealDelegate ReceivedHealDelegate;

protected:
	virtual void AbilitySpecInputPressed(FGameplayAbilitySpec& Spec) override;
	virtual void AbilitySpecInputReleased(FGameplayAbilitySpec& Spec) override;

	virtual void NotifyAbilityActivated(const FGameplayAbilitySpecHandle Handle, UGameplayAbility* Ability) override;
	virtual void NotifyAbilityEnded(FGameplayAbilitySpecHandle Handle, UGameplayAbility* Ability, bool bWasCancelled) override;
protected:
	// Handles to abilities that had their input pressed this frame.
	TArray<FGameplayAbilitySpecHandle> InputPressedSpecHandles;

	// Handles to abilities that had their input released this frame.
	TArray<FGameplayAbilitySpecHandle> InputReleasedSpecHandles;

	// Handles to abilities that have their input held.
	TArray<FGameplayAbilitySpecHandle> InputHeldSpecHandles;
};
