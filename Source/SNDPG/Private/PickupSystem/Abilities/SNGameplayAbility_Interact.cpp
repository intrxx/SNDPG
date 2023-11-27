// Fill out your copyright notice in the Description page of Project Settings.


#include "PickupSystem/Abilities/SNGameplayAbility_Interact.h"
#include "AbilitySystemComponent.h"
#include "PickupSystem/SNInteractionStatics.h"
#include "PickupSystem/SNInteractionOption.h"
#include "PickupSystem/SNInteractableTarget.h"
#include "NativeGameplayTags.h"
#include "Blueprint/UserWidget.h"
#include "Characters/Hero/Miscellaneous/SNHeroController.h"
#include "GAS/SNGameplayAbility.h"
#include "PickupSystem/Tasks/SNAbilityTask_GrantInteraction.h"

UE_DEFINE_GAMEPLAY_TAG_STATIC(TAG_Ability_Interaction_Activate, "Ability.Interaction.Activate");

UGameplayAbility_Interact::UGameplayAbility_Interact(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	ActivationPolicy = ESNAbilityActivationPolicy::OnSpawn;

	DefaultInteractionWidgetClass = StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("/Game/SNDPG/UI/Hero/Interact/Pickup/WBP_PickupPopup"));
	if(!DefaultInteractionWidgetClass)
	{
		UE_LOG(LogTemp, Error, TEXT("%s Failed to find DefaultInteractionWidgetClass."), *FString(__FUNCTION__));
	}
}

void UGameplayAbility_Interact::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	UAbilitySystemComponent* AbilitySystem = GetAbilitySystemComponentFromActorInfo();
	if (AbilitySystem && AbilitySystem->GetOwnerRole() == ROLE_Authority)
	{
		USNAbilityTask_GrantInteraction* Task = USNAbilityTask_GrantInteraction::GrantAbilitiesForNearbyInteractors(this, InteractionScanRange, InteractionScanRate);
		Task->ReadyForActivation();
	}
}

void UGameplayAbility_Interact::UpdateInteractions(const TArray<FSNInteractionOption>& InteractiveOptions)
{
	if (ASNHeroController* PC = GetSNPlayerControllerFromActorInfo())
	{
		if(!Widgets.IsEmpty())
		{
			//GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, FString::Printf(TEXT("Lengh of the array: %d"),Widgets.Num()));
			for(int32 i = 0; i < Widgets.Num(); i++)
			{
				Widgets[i]->RemoveFromParent();
			}
		}
		Widgets.Reset();
		
		for (const FSNInteractionOption& InteractionOption : InteractiveOptions)
		{
			AActor* InteractableTargetActor = USNInteractionStatics::GetActorFromInteractableTarget(InteractionOption.InteractableTarget);

			/*
			GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Yellow,
				FString::Printf(TEXT("Interaction Target: %s"), *InteractableTargetActor->GetName()));
			*/
			TSoftClassPtr<UUserWidget> InteractionWidgetClass = 
				InteractionOption.InteractionWidgetClass.IsNull() ? DefaultInteractionWidgetClass : InteractionOption.InteractionWidgetClass;

			if(InteractionWidgetClass.IsValid())
			{
				Widgets.Add(CreateWidget(GetWorld(), InteractionWidgetClass.Get()));
				Widgets.Last()->AddToViewport();
			}
		}
	}

	CurrentOptions = InteractiveOptions;
}

void UGameplayAbility_Interact::TriggerInteraction()
{
	if (CurrentOptions.Num() == 0)
	{
		return;
	}

	UAbilitySystemComponent* AbilitySystem = GetAbilitySystemComponentFromActorInfo();
	if (AbilitySystem)
	{
		const FSNInteractionOption& InteractionOption = CurrentOptions[0];

		AActor* Instigator = GetAvatarActorFromActorInfo();
		AActor* InteractableTargetActor = USNInteractionStatics::GetActorFromInteractableTarget(InteractionOption.InteractableTarget);

		// Allow the target to customize the event data we're about to pass in, in case the ability needs custom data
		// that only the actor knows.
		FGameplayEventData Payload;
		Payload.EventTag = TAG_Ability_Interaction_Activate;
		Payload.Instigator = Instigator;
		Payload.Target = InteractableTargetActor;

		// If needed we allow the interactable target to manipulate the event data so that for example, a button on the wall
		// may want to specify a door actor to execute the ability on, so it might choose to override Target to be the
		// door actor.
		InteractionOption.InteractableTarget->CustomizeInteractionEventData(TAG_Ability_Interaction_Activate, Payload);

		// Grab the target actor off the payload we're going to use it as the 'avatar' for the interaction, and the
		// source InteractableTarget actor as the owner actor.
		AActor* TargetActor = const_cast<AActor*>(ToRawPtr(Payload.Target));

		// The actor info needed for the interaction.
		FGameplayAbilityActorInfo ActorInfo;
		ActorInfo.InitFromActor(InteractableTargetActor, TargetActor, InteractionOption.TargetAbilitySystem);

		UE_LOG(LogTemp, Warning, TEXT("Trying to activate collect from event"));
		// Trigger the ability using event tag.
		const bool bSuccess = InteractionOption.TargetAbilitySystem->TriggerAbilityFromGameplayEvent(
			InteractionOption.TargetInteractionAbilityHandle,
			&ActorInfo,
			TAG_Ability_Interaction_Activate,
			&Payload,
			*InteractionOption.TargetAbilitySystem
		);
	}
}
