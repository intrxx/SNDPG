// Copyright 2023 Michal Oginski.


#include "GAS/SNAbilitySystemComponent.h"
#include "Characters/Hero/Miscellaneous/SNHeroController.h"
#include "Characters/Hero/SNHero.h"
#include "Characters/Hero/Miscellaneous/SNHeroState.h"
#include "GameplayTags/SNGameplayTags.h"
#include "GAS/SNGameplayAbility.h"
#include "GAS/SNGameplayAbility_Melee.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

USNAbilitySystemComponent::USNAbilitySystemComponent()
{
	InputPressedSpecHandles.Reset();
	InputReleasedSpecHandles.Reset();
	InputHeldSpecHandles.Reset();

	UE_LOG(LogTemp, Warning, TEXT("Enetered ASC contructror"));
}

void USNAbilitySystemComponent::AbilityInputTagPressed(const FGameplayTag& InputTag)
{
	if (InputTag.IsValid())
	{
		for (const FGameplayAbilitySpec& AbilitySpec : ActivatableAbilities.Items)
		{
			if (AbilitySpec.Ability && (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag)))
			{
				InputPressedSpecHandles.AddUnique(AbilitySpec.Handle);
				InputHeldSpecHandles.AddUnique(AbilitySpec.Handle);
			}
		}
	}
}

void USNAbilitySystemComponent::AbilityInputTagReleased(const FGameplayTag& InputTag)
{
	if (InputTag.IsValid())
	{
		for (const FGameplayAbilitySpec& AbilitySpec : ActivatableAbilities.Items)
		{
			if (AbilitySpec.Ability && (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag)))
			{
				InputReleasedSpecHandles.AddUnique(AbilitySpec.Handle);
				InputHeldSpecHandles.Remove(AbilitySpec.Handle);
			}
		}
	}
}

void USNAbilitySystemComponent::ProcessAbilityInput(float DeltaTime, bool bGamePaused)
{
	/*
	if (HasMatchingGameplayTag(TAG_Gameplay_AbilityInputBlocked))
	{
		ClearAbilityInput();
		return;
	}
	*/
	
	static TArray<FGameplayAbilitySpecHandle> AbilitiesToActivate;
	AbilitiesToActivate.Reset();
	
	//
	// Process all abilities that activate when the input is held.
	//
	for (const FGameplayAbilitySpecHandle& SpecHandle : InputHeldSpecHandles)
	{
		if (const FGameplayAbilitySpec* AbilitySpec = FindAbilitySpecFromHandle(SpecHandle))
		{
			if (AbilitySpec->Ability && !AbilitySpec->IsActive())
			{
				const USNGameplayAbility* SNAbility = CastChecked<USNGameplayAbility>(AbilitySpec->Ability);

				if (SNAbility->GetActivationPolicy() == ESNAbilityActivationPolicy::WhileInputActive)
				{
					AbilitiesToActivate.AddUnique(AbilitySpec->Handle);
				}
			}
		}
	}

	//
	// Process all abilities that had their input pressed this frame.
	//
	for (const FGameplayAbilitySpecHandle& SpecHandle : InputPressedSpecHandles)
	{
		if (FGameplayAbilitySpec* AbilitySpec = FindAbilitySpecFromHandle(SpecHandle))
		{
			if (AbilitySpec->Ability)
			{
				AbilitySpec->InputPressed = true;

				if (AbilitySpec->IsActive())
				{
					// Ability is active so pass along the input event.
					AbilitySpecInputPressed(*AbilitySpec);
				}
				else
				{
					const USNGameplayAbility* SNAbility = CastChecked<USNGameplayAbility>(AbilitySpec->Ability);

					if (SNAbility->GetActivationPolicy() == ESNAbilityActivationPolicy::OnInputTriggered)
					{
						AbilitiesToActivate.AddUnique(AbilitySpec->Handle);
					}
				}
			}
		}
	}

	//
	// Try to activate all the abilities that are from presses and holds.
	// We do it all at once so that held inputs don't activate the ability
	// and then also send a input event to the ability because of the press.
	//
	for (const FGameplayAbilitySpecHandle& AbilitySpecHandle : AbilitiesToActivate)
	{
		TryActivateAbility(AbilitySpecHandle);
	}
	
	//
	// Process all abilities that had their input released this frame.
	//
	for (const FGameplayAbilitySpecHandle& SpecHandle : InputReleasedSpecHandles)
	{
		if (FGameplayAbilitySpec* AbilitySpec = FindAbilitySpecFromHandle(SpecHandle))
		{
			if (AbilitySpec->Ability)
			{
				AbilitySpec->InputPressed = false;

				if (AbilitySpec->IsActive())
				{
					// Ability is active so pass along the input event.
					AbilitySpecInputReleased(*AbilitySpec);
				}
			}
		}
	}

	//
	// Clear the cached ability handles.
	//
	InputPressedSpecHandles.Reset();
	InputReleasedSpecHandles.Reset();
}

void USNAbilitySystemComponent::ClearAbilityInput()
{
	InputPressedSpecHandles.Reset();
	InputReleasedSpecHandles.Reset();
	InputHeldSpecHandles.Reset();
}

void USNAbilitySystemComponent::ReceivedDamage(USNAbilitySystemComponent* SourceASC, float UnmitigatedDamage,
                                               float MitigatedDamage)
{
	ReceivedDamageDelegate.Broadcast(SourceASC, UnmitigatedDamage, MitigatedDamage);
}

void USNAbilitySystemComponent::ReceivedHeal(USNAbilitySystemComponent* SourceASC, float HealingDone)
{
	ReceivedHealDelegate.Broadcast(SourceASC, HealingDone);
}

void USNAbilitySystemComponent::AbilitySpecInputPressed(FGameplayAbilitySpec& Spec)
{
	Super::AbilitySpecInputPressed(Spec);
	if (Spec.IsActive())
	{
		// Invoke the InputPressed event. This is not replicated here. If someone is listening, they may replicate the InputPressed event to the server.
		InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputPressed, Spec.Handle, Spec.ActivationInfo.GetActivationPredictionKey());
	}
}

void USNAbilitySystemComponent::AbilitySpecInputReleased(FGameplayAbilitySpec& Spec)
{
	Super::AbilitySpecInputReleased(Spec);
	if (Spec.IsActive())
	{
		// Invoke the InputReleased event. This is not replicated here. If someone is listening, they may replicate the InputReleased event to the server.
		InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputReleased, Spec.Handle, Spec.ActivationInfo.GetActivationPredictionKey());
	}
}

void USNAbilitySystemComponent::NotifyAbilityActivated(const FGameplayAbilitySpecHandle Handle,
	UGameplayAbility* Ability)
{
	Super::NotifyAbilityActivated(Handle, Ability);
	
	const FSNGameplayTags& GameplayTags = FSNGameplayTags::Get();
	
	if(Cast<USNGameplayAbility_Melee>(Ability))
	{
		AddLooseGameplayTag(GameplayTags.Ability_Behavior_BlockStaminaRegen, 1);

		ASNHeroState* PS = Cast<ASNHeroState>(GetOwner());
		if(PS)
		{
			ASNHeroController* PC = Cast<ASNHeroController>(PS->GetOwner());
			if(PC)
			{
				ASNHero* Hero = Cast<ASNHero>(PC->GetCharacter());
				if(Hero)
				{
					
					Hero->RemoveStaminaBlockTag(GetTagCount(GameplayTags.Ability_Behavior_BlockStaminaRegen));
				}
			}	
		}
	}
	
}

void USNAbilitySystemComponent::NotifyAbilityEnded(FGameplayAbilitySpecHandle Handle, UGameplayAbility* Ability,
                                                   bool bWasCancelled)
{
	Super::NotifyAbilityEnded(Handle, Ability, bWasCancelled);
	
}

