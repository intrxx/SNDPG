// Fill out your copyright notice in the Description page of Project Settings.


#include "PickupSystem/Tasks/SNAbilityTask_Scan_Trace.h"

#include "Characters/SNCharacterMovementComponent.h"
#include "Misc/TextFilterExpressionEvaluator.h"
#include "PickupSystem/SNInteractionStatics.h"

void USNAbilityTask_Scan_Trace::Activate()
{
	SetWaitingOnAvatar();

	UWorld* World = GetWorld();
	World->GetTimerManager().SetTimer(TimerHandle, this, &ThisClass::PerformTrace, InteractionScanRate, true);
}

USNAbilityTask_Scan_Trace* USNAbilityTask_Scan_Trace::ScanForInteractableTargets_Trace(
	UGameplayAbility* OwningAbility, FSNInteractionQuery InteractionQuery, FCollisionProfileName TraceProfile,
	FGameplayAbilityTargetingLocationInfo StartLocation, float InteractionScanRange, float InteractionScanRate,
	bool bShowDebug)
{
	USNAbilityTask_Scan_Trace* MyObj = NewAbilityTask<USNAbilityTask_Scan_Trace>(OwningAbility);
	MyObj->InteractionScanRange = InteractionScanRange;
	MyObj->InteractionScanRate = InteractionScanRate;
	MyObj->StartLocation = StartLocation;
	MyObj->InteractionQuery = InteractionQuery;
	MyObj->TraceProfile = TraceProfile;
	MyObj->bShowDebug = bShowDebug;

	return MyObj;
}

void USNAbilityTask_Scan_Trace::OnDestroy(bool AbilityEnded)
{
	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().ClearTimer(TimerHandle);
	}

	Super::OnDestroy(AbilityEnded);
}

void USNAbilityTask_Scan_Trace::PerformTrace()
{
	AActor* AvatarActor = Ability->GetCurrentActorInfo()->AvatarActor.Get();
	if (!AvatarActor)
	{
		return;
	}
	
	UWorld* World = GetWorld();

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(AvatarActor);

	const bool bTraceComplex = false;
	FCollisionQueryParams Params(SCENE_QUERY_STAT(USNAbilityTask_Scan_Trace), bTraceComplex);
	Params.AddIgnoredActors(ActorsToIgnore);
	
	FVector TraceStart = AvatarActor->GetActorLocation();
	FVector Direction = AvatarActor->GetActorForwardVector();
	Direction = Direction * InteractionScanRange;
	
	FVector TraceEnd = FVector(TraceStart.X, TraceStart.Y, TraceStart.Z - 100);
	//TraceEnd = Direction + TraceStart;
	//TraceEnd.Z = TraceEnd.Z - 50;
	
	FHitResult OutHitResult;
	LineTrace(OutHitResult, World, TraceStart, TraceEnd, TraceProfile.Name, Params);
	
	TArray<TScriptInterface<ISNInteractableTarget>> InteractableTargets;
	USNInteractionStatics::AppendInteractableTargetsFromHitResult(OutHitResult, InteractableTargets);

	UpdateInteractableOptions(InteractionQuery, InteractableTargets);
	
	if (bShowDebug)
	{
		FColor DebugColor = OutHitResult.bBlockingHit ? FColor::Red : FColor::Green;

		//DrawDebugBox(World, TraceStart, FVector(75), DebugColor, false, InteractionScanRate, 1,5);
		//DrawDebugLine(World, TraceStart, TraceEnd, DebugColor, false, InteractionScanRate);
		if (OutHitResult.bBlockingHit)
		{
			DrawDebugLine(World, TraceStart, OutHitResult.Location, DebugColor, false, InteractionScanRate);
			DrawDebugSphere(World, OutHitResult.Location, 5, 16, DebugColor, false, InteractionScanRate);
		}
		else
		{
			DrawDebugLine(World, TraceStart, TraceEnd, DebugColor, false, InteractionScanRate);
		}
	}
}
