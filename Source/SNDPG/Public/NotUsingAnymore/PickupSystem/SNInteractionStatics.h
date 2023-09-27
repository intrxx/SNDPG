// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "SNInteractionStatics.generated.h"

template <typename InterfaceType> class TScriptInterface;

class AActor;
class ISNInteractableTarget;
class UObject;
struct FFrame;
struct FHitResult;
struct FOverlapResult;

/**
 * 
 */
UCLASS()
class SNDPG_API USNInteractionStatics : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	USNInteractionStatics();

public:
	UFUNCTION(BlueprintCallable)
	static AActor* GetActorFromInteractableTarget(TScriptInterface<ISNInteractableTarget> InteractableTarget);

	UFUNCTION(BlueprintCallable)
	static void GetInteractableTargetsFromActor(AActor* Actor, TArray<TScriptInterface<ISNInteractableTarget>>& OutInteractableTargets);

	static void AppendInteractableTargetsFromOverlapResults(const TArray<FOverlapResult>& OverlapResults, TArray<TScriptInterface<ISNInteractableTarget>>& OutInteractableTargets);
	static void AppendInteractableTargetsFromHitResult(const FHitResult& HitResult, TArray<TScriptInterface<ISNInteractableTarget>>& OutInteractableTargets);
};
