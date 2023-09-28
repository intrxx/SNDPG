// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "CoreMinimal.h"
#include "PickupSystem/SNInteractionQuery.h"
#include "PickupSystem/Tasks/SNAbilityTask_ScanForInteract.h"
#include "SNAbilityTask_Scan_Trace.generated.h"

/**
 * 
 */
UCLASS()
class SNDPG_API USNAbilityTask_Scan_Trace : public USNAbilityTask_ScanForInteract
{
	GENERATED_BODY()

	virtual void Activate() override;

	/** Wait until we trace new set of interactables.  This task automatically loops. */
	UFUNCTION(BlueprintCallable, Category = "SNAbility|Tasks", meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
	static USNAbilityTask_Scan_Trace* ScanForInteractableTargets_Trace(UGameplayAbility* OwningAbility, FSNInteractionQuery InteractionQuery, FCollisionProfileName TraceProfile, FGameplayAbilityTargetingLocationInfo StartLocation, float InteractionScanRange = 100, float InteractionScanRate = 0.100, bool bShowDebug = false);

private:

	virtual void OnDestroy(bool AbilityEnded) override;

	void PerformTrace();

	UPROPERTY()
	FSNInteractionQuery InteractionQuery;

	UPROPERTY()
	FGameplayAbilityTargetingLocationInfo StartLocation;

	float InteractionScanRange = 250.f;
	float InteractionScanRate = 0.100;
	bool bShowDebug = false;

	FTimerHandle TimerHandle;
};
