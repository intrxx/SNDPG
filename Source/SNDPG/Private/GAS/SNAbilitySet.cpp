// Copyright 2023 Michal Oginski.

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

void FSNAbilitySet_GrantedHandles::TakeFromAbilitySystem(USNAbilitySystemComponent* InASC)
{
	check(InASC);

	if (!InASC->IsOwnerActorAuthoritative())
	{
		// Must be authoritative to give or take ability sets.
		return;
	}

	for (const FGameplayAbilitySpecHandle& Handle : AbilitySpecHandles)
	{
		if (Handle.IsValid())
		{
			InASC->ClearAbility(Handle);
		}
	}

	for (const FActiveGameplayEffectHandle& Handle : GameplayEffectHandles)
	{
		if (Handle.IsValid())
		{
			InASC->RemoveActiveGameplayEffect(Handle);
		}
	}

	for (UAttributeSet* Set : GrantedAttributeSets)
	{
		InASC->RemoveSpawnedAttribute(Set);
	}

	AbilitySpecHandles.Reset();
	GameplayEffectHandles.Reset();
	GrantedAttributeSets.Reset();
}

USNAbilitySet::USNAbilitySet()
{
}

void USNAbilitySet::GiveToAbilitySystem(USNAbilitySystemComponent* InASC,
	FSNAbilitySet_GrantedHandles* OutGrantedHandles, UObject* SourceObject) const
{
	check(InASC);

	if (!InASC->IsOwnerActorAuthoritative())
	{
		// Must be authoritative to give or take ability sets.
		return;
	}

	// Grant the gameplay abilities.
	if(InASC->bAbilitiesGiven == false)
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

			/*
			UE_LOG(LogTemp, Error, TEXT("------------------ Binding Tags to Abilities------------------"));
			UE_LOG(LogTemp, Warning, TEXT("Ability: %s"), *AbilityToGrant.Ability->GetName());
			UE_LOG(LogTemp, Warning, TEXT("Input Tag: %s"), *AbilityToGrant.InputTag.ToString());
			UE_LOG(LogTemp, Warning, TEXT("Input Tag On Spec: %s"), *AbilitySpec.DynamicAbilityTags.GetByIndex(0).ToString());
			*/
			
			const FGameplayAbilitySpecHandle AbilitySpecHandle = InASC->GiveAbility(AbilitySpec);

			if (OutGrantedHandles)
			{
				OutGrantedHandles->AddAbilitySpecHandle(AbilitySpecHandle);
			}
		}
	}
	InASC->bAbilitiesGiven = true;

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
		const FActiveGameplayEffectHandle GameplayEffectHandle = InASC->ApplyGameplayEffectToSelf(GameplayEffect, EffectToGrant.EffectLevel, InASC->MakeEffectContext());

		if (OutGrantedHandles)
		{
			OutGrantedHandles->AddGameplayEffectHandle(GameplayEffectHandle);
		}
	}

	// Grant the attribute sets.
	if(InASC->bAttributesGiven == false)
	{
		for (int32 SetIndex = 0; SetIndex < GrantedAttributes.Num(); ++SetIndex)
		{
			const FSNAbilitySet_AttributeSet& SetToGrant = GrantedAttributes[SetIndex];

			if (!IsValid(SetToGrant.AttributeSet))
			{
				UE_LOG(LogTemp, Error, TEXT("GrantedAttributes[%d] on ability set [%s] is not valid"), SetIndex, *GetNameSafe(this));
				continue;
			}

			UAttributeSet* NewSet = NewObject<UAttributeSet>(InASC->GetOwner(), SetToGrant.AttributeSet);
			InASC->AddAttributeSetSubobject(NewSet);

			if (OutGrantedHandles)
			{
				//OutGrantedHandles->AddAttributeSet(NewSet);
			}
		}
	}
	InASC->bAttributesGiven = true;
}
