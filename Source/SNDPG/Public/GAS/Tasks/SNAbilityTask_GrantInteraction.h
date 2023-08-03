// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "SNAbilityTask_GrantInteraction.generated.h"

/**
 * 
 */
UCLASS()
class SNDPG_API USNAbilityTask_GrantInteraction : public UAbilityTask
{
	GENERATED_UCLASS_BODY()

	virtual void Activate() override;

	UFUNCTION(BlueprintCallable, Category="Ability|Tasks", meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
	static USNAbilityTask_GrantInteraction* GrantAbilitiesForNearbyInteractors(UGameplayAbility* OwningAbility, float InteractionScanRange, float InteractionScanRate);

private:
	virtual void OnDestroy(bool bInOwnerFinished) override;

	void QueryInteractables();

	float InteractionScanRange = 100;
	float InteractionScanRate = 0.100;

	FTimerHandle QueryTimerHandle;

	TMap<FObjectKey, FGameplayAbilitySpecHandle> InteractionAbilityCache;
};
