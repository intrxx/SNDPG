// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Abilities/SNGameplayAbility_BarghestBite.h"

#include "ActorComponents/SNBasicAttributesComponent.h"
#include "Characters/Enemy/SNEnemy.h"

USNGameplayAbility_BarghestBite::USNGameplayAbility_BarghestBite(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

ASNEnemy* USNGameplayAbility_BarghestBite::GetBarghestFromActorInfo() const
{
	return (CurrentActorInfo ? Cast<ASNEnemy>(CurrentActorInfo->AvatarActor.Get()) : nullptr);
}

void USNGameplayAbility_BarghestBite::SetDamageAndMontage()
{
	ASNEnemy* EnemyOwner = GetBarghestFromActorInfo();

	const USNBasicAttributesComponent* AttributesComp = USNBasicAttributesComponent::FindAttributeComponent(EnemyOwner);
	if(AttributesComp)
	{
		if (AttributesComp->GetHealth() > AttributesComp->GetMaxHealth() / 2)
		{
			if(HighLifeAnimMontage)
			{
				AnimMontageToUse = HighLifeAnimMontage;
			}
			DamageToUse = HighLifeDamage;
		}
		else
		{
			if(LowLifeAnimMontage)
			{
				AnimMontageToUse = LowLifeAnimMontage;
			}
			DamageToUse = LowLifeDamage;
		}
	}
}
