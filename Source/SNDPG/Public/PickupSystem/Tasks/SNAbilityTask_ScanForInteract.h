// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "PickupSystem/SNInteractionOption.h"

#include "SNAbilityTask_ScanForInteract.generated.h"

class ISNInteractableTarget;
struct FSNInteractionQuery;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInteractableObjectsChangedEvent, const TArray<FSNInteractionOption>&, InteractableOptions);
/**
 * 
 */
UCLASS(Abstract)
class SNDPG_API USNAbilityTask_ScanForInteract : public UAbilityTask
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FInteractableObjectsChangedEvent InteractableObjectsChanged;

	void UpdateInteractableOptions(const FSNInteractionQuery& InteractQuery, const TArray<TScriptInterface<ISNInteractableTarget>>& InteractableTargets);
	static void BoxTrace(FHitResult& OutHitResult, const UWorld* World, const FVector& Start, const FVector& End, FName ProfileName, TArray<AActor*>& ActorsToIgnore);
	static void LineTrace(FHitResult& OutHitResult, const UWorld* World, const FVector& Start, const FVector& End, FName ProfileName, const FCollisionQueryParams Params);
	void AimWithPlayerRotation(const AActor* InSourceActor, FCollisionQueryParams Params, const FVector& TraceStart, float MaxRange, FVector& OutTraceEnd, bool bIgnorePitch = false) const;
	
protected:
	TArray<FSNInteractionOption> CurrentOptions;
	
	FCollisionProfileName TraceProfile;
};
