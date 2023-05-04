// Fill out your copyright notice in the Description page of Project Settings.

#include "GAS/SNAbilitySet.h"
#include "GAS/SNAbilitySystemComponent.h"
#include "GAS/SNGameplayAbility.h"


void FSNAbilitySet_GrantedHandles::AddAbilitySpecHandle(const FGameplayAbilitySpecHandle& Handle)
{
	if (Handle.IsValid())
	{
		AbilitySpecHandles.Add(Handle);
	}
}

void FSNAbilitySet_GrantedHandles::AddGameplayEffectHandle(const FActiveGameplayEffectHandle& Handle)
{
	if (Handle.IsValid())
	{
		GameplayEffectHandles.Add(Handle);
	}
}

void FSNAbilitySet_GrantedHandles::AddAttributeSet(UAttributeSet* Set)
{
	GrantedAttributeSets.Add(Set);
}

void FSNAbilitySet_GrantedHandles::TakeFromAbilitySystem(USNAbilitySystemComponent* SNASC)
{
	check(SNASC);

	if (!SNASC->IsOwnerActorAuthoritative())
	{
		// Must be authoritative to give or take ability sets.
		return;
	}

	for (const FGameplayAbilitySpecHandle& Handle : AbilitySpecHandles)
	{
		if (Handle.IsValid())
		{
			SNASC->ClearAbility(Handle);
		}
	}

	for (const FActiveGameplayEffectHandle& Handle : GameplayEffectHandles)
	{
		if (Handle.IsValid())
		{
			SNASC->RemoveActiveGameplayEffect(Handle);
		}
	}

	for (UAttributeSet* Set : GrantedAttributeSets)
	{
		SNASC->RemoveSpawnedAttribute(Set);
	}

	AbilitySpecHandles.Reset();
	GameplayEffectHandles.Reset();
	GrantedAttributeSets.Reset();
}

USNAbilitySet::USNAbilitySet()
{
}

void USNAbilitySet::GiveToAbilitySystem(USNAbilitySystemComponent* SNASC,
	FSNAbilitySet_GrantedHandles* OutGrantedHandles, UObject* SourceObject) const
{
	check(SNASC);

	if (!SNASC->IsOwnerActorAuthoritative())
	{
		// Must be authoritative to give or take ability sets.
		return;
	}

	// Grant the gameplay abilities.
	if(SNASC->bAbilitiesGiven == false)
	{
		for (int32 AbilityIndex = 0; AbilityIndex < GrantedGameplayAbilities.Num(); ++AbilityIndex)
		{
			const FSNAbilitySet_GameplayAbility& AbilityToGrant = GrantedGameplayAbilities[AbilityIndex];

			if (!IsValid(AbilityToGrant.Ability))
			{
				UE_LOG(LogTemp, Error, TEXT("GrantedGameplayAbilities[%d] on ability set [%s] is not valid."), AbilityIndex, *GetNameSafe(this));
				continue;
			}

			USNGameplayAbility* AbilityCDO = AbilityToGrant.Ability->GetDefaultObject<USNGameplayAbility>();

			FGameplayAbilitySpec AbilitySpec(AbilityCDO, AbilityToGrant.AbilityLevel);
			AbilitySpec.SourceObject = SourceObject;
			AbilitySpec.DynamicAbilityTags.AddTag(AbilityToGrant.InputTag);

			const FGameplayAbilitySpecHandle AbilitySpecHandle = SNASC->GiveAbility(AbilitySpec);

			if (OutGrantedHandles)
			{
				OutGrantedHandles->AddAbilitySpecHandle(AbilitySpecHandle);
			}
		}
	}
	SNASC->bAbilitiesGiven = true;

	// Grant the gameplay effects.
	for (int32 EffectIndex = 0; EffectIndex < GrantedGameplayEffects.Num(); ++EffectIndex)
	{
		const FSNAbilitySet_GameplayEffect& EffectToGrant = GrantedGameplayEffects[EffectIndex];

		if (!IsValid(EffectToGrant.GameplayEffect))
		{
			UE_LOG(LogTemp, Error, TEXT("GrantedGameplayEffects[%d] on ability set [%s] is not valid"), EffectIndex, *GetNameSafe(this));
			continue;
		}

		const UGameplayEffect* GameplayEffect = EffectToGrant.GameplayEffect->GetDefaultObject<UGameplayEffect>();
		const FActiveGameplayEffectHandle GameplayEffectHandle = SNASC->ApplyGameplayEffectToSelf(GameplayEffect, EffectToGrant.EffectLevel, SNASC->MakeEffectContext());

		if (OutGrantedHandles)
		{
			OutGrantedHandles->AddGameplayEffectHandle(GameplayEffectHandle);
		}
	}

	// Grant the attribute sets.
	if(SNASC->bAttributesGiven == false)
	{
		for (int32 SetIndex = 0; SetIndex < GrantedAttributes.Num(); ++SetIndex)
		{
			const FSNAbilitySet_AttributeSet& SetToGrant = GrantedAttributes[SetIndex];

			if (!IsValid(SetToGrant.AttributeSet))
			{
				UE_LOG(LogTemp, Error, TEXT("GrantedAttributes[%d] on ability set [%s] is not valid"), SetIndex, *GetNameSafe(this));
				continue;
			}

			UAttributeSet* NewSet = NewObject<UAttributeSet>(SNASC->GetOwner(), SetToGrant.AttributeSet);
			SNASC->AddAttributeSetSubobject(NewSet);

			if (OutGrantedHandles)
			{
				//OutGrantedHandles->AddAttributeSet(NewSet);
			}
		}
	}
	SNASC->bAttributesGiven = true;
}
