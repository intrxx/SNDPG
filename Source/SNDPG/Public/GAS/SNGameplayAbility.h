// Copyright 2023 Michal Oginski.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "SNGameplayAbility.generated.h"

/**
 * Defines how ability is meant to activate
 */
UENUM(BlueprintType)
enum class ESNAbilityActivationPolicy : uint8
{
	// Try to activate the ability when the input is triggered.
	OnInputTriggered,
	
	// Continually try to activate the ability while the input is active.
	WhileInputActive,
	
	// Try to activate the ability when an avatar is assigned.
	OnSpawn
};
UCLASS()
class SNDPG_API USNGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	USNGameplayAbility(const FObjectInitializer& ObjectInitializer);

	ESNAbilityActivationPolicy GetActivationPolicy() const {return ActivationPolicy;}

	void TryActivateAbilityOnSpawn(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) const;

protected:
	virtual void OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;

	/** Called when this ability is granted to the ability system component. */
	UFUNCTION(BlueprintImplementableEvent, Category = Ability, DisplayName = "OnAbilityAdded")
	void K2_OnAbilityAdded();

protected:
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability Activation")
	ESNAbilityActivationPolicy ActivationPolicy;
};
