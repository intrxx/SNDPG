// Fill out your copyright notice in the Description page of Project Settings.


#include "NotUsingAnymore/PickupSystem/Tasks/SNAbilityTask_GrantInteraction.h"
#include "Engine/World.h"
#include "GameFramework/Controller.h"
#include "AbilitySystemComponent.h"
#include "NotUsingAnymore/PickupSystem/SNInteractableTarget.h"
#include "NotUsingAnymore/PickupSystem/SNInteractionOption.h"
#include "NotUsingAnymore/PickupSystem/SNInteractionQuery.h"
#include "NotUsingAnymore/PickupSystem/SNInteractionStatics.h"
#include "Physics/SNDPGCollisionChannels.h"
#include "TimerManager.h"


USNAbilityTask_GrantInteraction::USNAbilityTask_GrantInteraction(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void USNAbilityTask_GrantInteraction::Activate()
{
	SetWaitingOnAvatar();

	UWorld* World = GetWorld();
	World->GetTimerManager().SetTimer(QueryTimerHandle, this, &ThisClass::QueryInteractables, InteractionScanRate, true);
}

USNAbilityTask_GrantInteraction* USNAbilityTask_GrantInteraction::GrantAbilitiesForNearbyInteractors(
	UGameplayAbility* OwningAbility, float InteractionScanRange, float InteractionScanRate)
{
	USNAbilityTask_GrantInteraction* MyObj = NewAbilityTask<USNAbilityTask_GrantInteraction>(OwningAbility);
	MyObj->InteractionScanRange = InteractionScanRange;
	MyObj->InteractionScanRate = InteractionScanRate;
	return MyObj;
}

void USNAbilityTask_GrantInteraction::OnDestroy(bool bInOwnerFinished)
{
	if(UWorld* World = GetWorld())
	{
		World->GetTimerManager().ClearTimer(QueryTimerHandle);
	}
	
	Super::OnDestroy(bInOwnerFinished);
}

void USNAbilityTask_GrantInteraction::QueryInteractables()
{
	UWorld* World = GetWorld();
	AActor* ActorOwner = GetAvatarActor();

	if (World && ActorOwner)
	{
		FCollisionQueryParams Params(SCENE_QUERY_STAT(UAbilityTask_GrantNearbyInteraction), false);

		TArray<FOverlapResult> OverlapResults;
		
		World->OverlapMultiByChannel(OUT OverlapResults, ActorOwner->GetActorLocation(), FQuat::Identity, SNDPG_TraceChannel_Interaction, FCollisionShape::MakeSphere(InteractionScanRange), Params);

		if (OverlapResults.Num() > 0)
		{
			TArray<TScriptInterface<ISNInteractableTarget>> InteractableTargets;
			USNInteractionStatics::AppendInteractableTargetsFromOverlapResults(OverlapResults, OUT InteractableTargets);
			
			FSNInteractionQuery InteractionQuery;
			InteractionQuery.RequestingAvatar = ActorOwner;
			InteractionQuery.RequestingController = Cast<AController>(ActorOwner->GetOwner());

			TArray<FSNInteractionOption> Options;
			for (TScriptInterface<ISNInteractableTarget>& InteractiveTarget : InteractableTargets)
			{
				FSNInteractionOptionBuilder InteractionBuilder(InteractiveTarget, Options);
				InteractiveTarget->GatherInteractionOptions(InteractionQuery, InteractionBuilder);
			}

			// Check if any of the options need to grant the ability to the user before they can be used.
			for (FSNInteractionOption& Option : Options)
			{
				if (Option.InteractionAbilityToGrant)
				{
					// Grant the ability to the GAS, otherwise it won't be able to do whatever the interaction is.
					FObjectKey ObjectKey(Option.InteractionAbilityToGrant);
					if (!InteractionAbilityCache.Find(ObjectKey))
					{
						FGameplayAbilitySpec Spec(Option.InteractionAbilityToGrant, 1, INDEX_NONE, this);
						FGameplayAbilitySpecHandle Handle = AbilitySystemComponent->GiveAbility(Spec);
						InteractionAbilityCache.Add(ObjectKey, Handle);
					}
				}
			}
		}
	}
}
