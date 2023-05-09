// Copyright 2023 Michal Oginski.


#include "Characters/Enemy/SNEnemy.h"
#include "GAS/SNAbilitySet.h"
#include "GAS/SNGameplayAbility.h"
#include "Characters/Hero/Miscellaneous/SNBasicAttributesComponent.h"
#include "GAS/SNAbilitySystemComponent.h"
#include "GAS/Attributes/SNBasicAttributes.h"

ASNEnemy::ASNEnemy(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	EnemyAbilitySystemComponent = CreateDefaultSubobject<USNAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	EnemyAbilitySystemComponent->SetIsReplicated(true);
	EnemyAbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);
	
	AbilitySystemComponent = EnemyAbilitySystemComponent;
	
	AttributesComponent = CreateDefaultSubobject<USNBasicAttributesComponent>(TEXT("BasicAttributesComponent"));
}

USNAbilitySystemComponent* ASNEnemy::GetEnemyAbilitySystemComponent() const
{
	return EnemyAbilitySystemComponent;
}

void ASNEnemy::BeginPlay()
{
	Super::BeginPlay();

	if(AbilitySystemComponent.IsValid())
	{
		AbilitySystemComponent->InitAbilityActorInfo(this,this);
		AbilitySet->GiveToAbilitySystem(AbilitySystemComponent.Get(), nullptr, nullptr);
		AttributesComponent->InitializeWithAbilitySystem(AbilitySystemComponent.Get());
		
	}
}

void ASNEnemy::HealthChanged(const FOnAttributeChangeData& Data)
{
}


