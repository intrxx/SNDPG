// Fill out your copyright notice in the Description page of Project Settings.

#include "PickupSystem/Tasks/SNAbilityTask_ScanForInteract.h"
#include "PickupSystem/SNInteractableTarget.h"
#include "AbilitySystemComponent.h"
#include "Kismet/KismetSystemLibrary.h"


void USNAbilityTask_ScanForInteract::UpdateInteractableOptions(const FSNInteractionQuery& InteractQuery,
                                                               const TArray<TScriptInterface<ISNInteractableTarget>>& InteractableTargets)
{
	TArray<FSNInteractionOption> NewOptions;

	for (const TScriptInterface<ISNInteractableTarget>& InteractiveTarget : InteractableTargets)
	{
		TArray<FSNInteractionOption> TempOptions;
		FSNInteractionOptionBuilder InteractionBuilder(InteractiveTarget, TempOptions);
		InteractiveTarget->GatherInteractionOptions(InteractQuery, InteractionBuilder);

		for (FSNInteractionOption& Option : TempOptions)
		{
			FGameplayAbilitySpec* InteractionAbilitySpec = nullptr;

			// if there is a handle an a target ability system, we're triggering the ability on the target.
			if (Option.TargetAbilitySystem && Option.TargetInteractionAbilityHandle.IsValid())
			{
				// Find the spec
				InteractionAbilitySpec = Option.TargetAbilitySystem->FindAbilitySpecFromHandle(Option.TargetInteractionAbilityHandle);
			}
			// If there's an interaction ability then we're activating it on ourselves.
			else if (Option.InteractionAbilityToGrant)
			{
				
				// Find the spec
				InteractionAbilitySpec = AbilitySystemComponent->FindAbilitySpecFromClass(Option.InteractionAbilityToGrant);

				if (InteractionAbilitySpec)
				{
					// update the option
					Option.TargetAbilitySystem = AbilitySystemComponent.Get();
					Option.TargetInteractionAbilityHandle = InteractionAbilitySpec->Handle;
				}
			}

			if (InteractionAbilitySpec)
			{
				// Filter any options that we can't activate right now for whatever reason.
				if (InteractionAbilitySpec->Ability->CanActivateAbility(InteractionAbilitySpec->Handle, AbilitySystemComponent->AbilityActorInfo.Get()))
				{
					NewOptions.Add(Option);
				}
			}
		}
	}

	bool bOptionsChanged = false;
	if (NewOptions.Num() == CurrentOptions.Num())
	{
		NewOptions.Sort();

		for (int OptionIndex = 0; OptionIndex < NewOptions.Num(); OptionIndex++)
		{
			const FSNInteractionOption& NewOption = NewOptions[OptionIndex];
			const FSNInteractionOption& CurrentOption = CurrentOptions[OptionIndex];

			if (NewOption != CurrentOption)
			{
				bOptionsChanged = true;
				break;
			}
		}
	}
	else
	{
		bOptionsChanged = true;
	}

	if (bOptionsChanged)
	{
		CurrentOptions = NewOptions;
		InteractableObjectsChanged.Broadcast(CurrentOptions);
	}
}

void USNAbilityTask_ScanForInteract::BoxTrace(FHitResult& OutHitResult, const UWorld* World, const FVector& Start,
	const FVector& End, FName ProfileName, TArray<AActor*>& ActorsToIgnore)
{
	check(World);

	OutHitResult = FHitResult();
	TArray<FHitResult> HitResults;
	/* Line Trace 
	World->LineTraceMultiByProfile(HitResults, Start, End, ProfileName, Params);
	*/
	UKismetSystemLibrary::BoxTraceMultiByProfile(World, Start, End, FVector(50), FRotator(),
		ProfileName, true, ActorsToIgnore, EDrawDebugTrace::None, HitResults, true);
	
	OutHitResult.TraceStart = Start;
	OutHitResult.TraceEnd = End;

	if (HitResults.Num() > 0)
	{
		OutHitResult = HitResults[0];
	}
}

void USNAbilityTask_ScanForInteract::LineTrace(FHitResult& OutHitResult, const UWorld* World, const FVector& Start,
	const FVector& End, FName ProfileName, const FCollisionQueryParams Params)
{
	check(World);

	OutHitResult = FHitResult();
	TArray<FHitResult> HitResults;
	
	World->LineTraceMultiByProfile(HitResults, Start, End, ProfileName, Params);
	
	OutHitResult.TraceStart = Start;
	OutHitResult.TraceEnd = End;

	if (HitResults.Num() > 0)
	{
		OutHitResult = HitResults[0];
	}
}

void USNAbilityTask_ScanForInteract::AimWithPlayerRotation(const AActor* InSourceActor, FCollisionQueryParams Params,
                                                           const FVector& TraceStart, float MaxRange, FVector& OutTraceEnd, bool bIgnorePitch) const
{
	if (!Ability) // Server and launching client only
		{
		return;
		}

	//@TODO: Bots?
	APlayerController* PC = Ability->GetCurrentActorInfo()->PlayerController.Get();
	check(PC);

	FVector ViewStart;
	FRotator ViewRot;
	PC->GetPlayerViewPoint(ViewStart, ViewRot);
	
	
	
	//UE_LOG(LogTemp, Warning, TEXT("ViewStar x: %f y: %f z: %f"), TraceStart.X, TraceStart.Y, TraceStart.Z);
	UE_LOG(LogTemp, Warning, TEXT("ViewStar x: %f y: %f z: %f"), TraceStart.X, TraceStart.Y, TraceStart.Z);
	
	//UE_LOG(LogTemp, Warning, TEXT("ViewStar x: %f y: %f z: %f"), ViewStart.X, ViewStart.Y, ViewStart.Z)

	const FVector ViewDir = ViewRot.Vector();
	FVector ViewEnd = ViewStart + (ViewDir * MaxRange);
	
	FHitResult HitResult;

	const bool bUseTraceResult = HitResult.bBlockingHit && (FVector::DistSquared(TraceStart, HitResult.Location) <= (MaxRange * MaxRange));

	const FVector AdjustedEnd = (bUseTraceResult) ? HitResult.Location : ViewEnd;

	FVector AdjustedAimDir = (AdjustedEnd - TraceStart).GetSafeNormal();
	if (AdjustedAimDir.IsZero())
	{
		AdjustedAimDir = ViewDir;
	}
	
	OutTraceEnd = TraceStart + MaxRange;

	FVector ActorForwardVector = InSourceActor->GetActorForwardVector();
	OutTraceEnd = FVector(OutTraceEnd.X * ActorForwardVector.X, OutTraceEnd.Y * ActorForwardVector.Y, OutTraceEnd.Z);
}
