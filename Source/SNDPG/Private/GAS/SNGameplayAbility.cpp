// Copyright 2023 Michal Oginski.


#include "GAS/SNGameplayAbility.h"
#include "Characters/SNCharacterMovementComponent.h"
#include "Characters/Hero/SNHero.h"
#include "Characters/Hero/Miscellaneous/SNHeroController.h"
#include "GAS/SNAbilitySystemComponent.h"

USNGameplayAbility::USNGameplayAbility(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

USNAbilitySystemComponent* USNGameplayAbility::GetSNAbilitySystemComponentFromActorInfo() const
{
	return (CurrentActorInfo ? Cast<USNAbilitySystemComponent>(CurrentActorInfo->AbilitySystemComponent.Get()) : nullptr);
}

ASNHeroController* USNGameplayAbility::GetSNPlayerControllerFromActorInfo() const
{
	return (CurrentActorInfo ? Cast<ASNHeroController>(CurrentActorInfo->PlayerController.Get()) : nullptr);
}

ASNHero* USNGameplayAbility::GetSNHeroFromActorInfo() const
{
	return (CurrentActorInfo ? Cast<ASNHero>(CurrentActorInfo->AvatarActor.Get()) : nullptr);
}

USNCharacterMovementComponent* USNGameplayAbility::GetSNCharacterMoveCompFromActorInfo()
{
	return (CurrentActorInfo ? Cast<USNCharacterMovementComponent>(CurrentActorInfo->MovementComponent.Get()) : nullptr);
}

void USNGameplayAbility::LockCharacterInPlace()
{
	GetSNPlayerControllerFromActorInfo()->SetIgnoreMoveInput(true);
}

void USNGameplayAbility::UnlockCharacter()
{
	GetSNPlayerControllerFromActorInfo()->ResetIgnoreInputFlags();
}

void USNGameplayAbility::TryActivateAbilityOnSpawn(const FGameplayAbilityActorInfo* ActorInfo,
                                                   const FGameplayAbilitySpec& Spec) const
{
	const bool bIsPredicting = (Spec.ActivationInfo.ActivationMode == EGameplayAbilityActivationMode::Predicting);

	// Try to activate if activation policy is on spawn.
	if (ActorInfo && !Spec.IsActive() && !bIsPredicting && (ActivationPolicy == ESNAbilityActivationPolicy::OnSpawn))
	{
		UAbilitySystemComponent* ASC = ActorInfo->AbilitySystemComponent.Get();
		const AActor* AvatarActor = ActorInfo->AvatarActor.Get();

		// If avatar actor is torn off or about to die, don't try to activate until we get the new one.
		if (ASC && AvatarActor && !AvatarActor->GetTearOff() && (AvatarActor->GetLifeSpan() <= 0.0f))
		{
			const bool bIsLocalExecution = (NetExecutionPolicy == EGameplayAbilityNetExecutionPolicy::LocalPredicted) || (NetExecutionPolicy == EGameplayAbilityNetExecutionPolicy::LocalOnly);
			const bool bIsServerExecution = (NetExecutionPolicy == EGameplayAbilityNetExecutionPolicy::ServerOnly) || (NetExecutionPolicy == EGameplayAbilityNetExecutionPolicy::ServerInitiated);

			const bool bClientShouldActivate = ActorInfo->IsLocallyControlled() && bIsLocalExecution;
			const bool bServerShouldActivate = ActorInfo->IsNetAuthority() && bIsServerExecution;

			if (bClientShouldActivate || bServerShouldActivate)
			{
				ASC->TryActivateAbility(Spec.Handle);
			}
		}
	}
}

void USNGameplayAbility::OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnGiveAbility(ActorInfo, Spec);

	K2_OnAbilityAdded();
	
	TryActivateAbilityOnSpawn(ActorInfo, Spec);
}
