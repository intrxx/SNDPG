// Copyright 2023 Michal Oginski.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "SNGameplayAbility.generated.h"

class USNCharacterMovementComponent;
class ASNHero;
class ASNHeroController;
class USNAbilitySystemComponent;
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

	UFUNCTION(BlueprintCallable, Category = "SN|Ability")
	USNAbilitySystemComponent* GetSNAbilitySystemComponentFromActorInfo() const;

	UFUNCTION(BlueprintCallable, Category = "SN|Ability")
	ASNHeroController* GetSNPlayerControllerFromActorInfo() const;

	UFUNCTION(BlueprintCallable, Category = "SN|Ability")
	ASNHero* GetSNHeroFromActorInfo() const;

	UFUNCTION(BlueprintCallable, Category = "SN|Ability")
	USNCharacterMovementComponent* GetSNCharacterMoveCompFromActorInfo();
	
	UFUNCTION(BlueprintCallable, Category = "SN|Ability")
	void LockCharacterInPlace();
	
	UFUNCTION(BlueprintCallable, Category = "SN|Ability")
	void UnlockCharacter();
	
	void TryActivateAbilityOnSpawn(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) const;

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SN|Ability Proprietes")
	bool bShouldBlockStaminaRegen = false;
	

protected:
	virtual void OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;

	/** Called when this ability is granted to the ability system component. */
	UFUNCTION(BlueprintImplementableEvent, Category = Ability, DisplayName = "OnAbilityAdded")
	void K2_OnAbilityAdded();

protected:
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SN|Ability Activation")
	ESNAbilityActivationPolicy ActivationPolicy;
};
