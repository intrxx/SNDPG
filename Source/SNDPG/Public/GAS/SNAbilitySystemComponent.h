// Copyright 2023 Michal Oginski.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "SNAbilitySystemComponent.generated.h"

/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FReceivedDamageDelegate, USNAbilitySystemComponent*, SourceASC, float, UnmitigatedDamage, float, MitigatedDamage);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FReceivedHealDelegate, USNAbilitySystemComponent*, SourceASC, float, UnmitigatedHeal, float, MitigatedHeal);

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
	virtual void ReceivedHeal(USNAbilitySystemComponent* SourceASC, float UnmitigatedHeal, float MitigatedHeal);
public:
	bool bAbilitiesGiven = false;
	bool bAttributesGiven = false;

	FReceivedDamageDelegate ReceivedDamageDelegate;
	FReceivedHealDelegate ReceivedHealDelegate;

protected:
	virtual void AbilitySpecInputPressed(FGameplayAbilitySpec& Spec) override;
	virtual void AbilitySpecInputReleased(FGameplayAbilitySpec& Spec) override;
	
protected:
	// Handles to abilities that had their input pressed this frame.
	TArray<FGameplayAbilitySpecHandle> InputPressedSpecHandles;

	// Handles to abilities that had their input released this frame.
	TArray<FGameplayAbilitySpecHandle> InputReleasedSpecHandles;

	// Handles to abilities that have their input held.
	TArray<FGameplayAbilitySpecHandle> InputHeldSpecHandles;
};
