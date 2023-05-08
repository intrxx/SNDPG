// Copyright 2023 Michal Oginski.


#include "Characters/Enemy/SNEnemy.h"
#include "Characters/Hero/Miscellaneous/SNBasicAttributesComponent.h"
#include "GAS/SNAbilitySet.h"
#include "GAS/SNAbilitySystemComponent.h"
#include "GAS/Attributes/SNBasicAttributes.h"

ASNEnemy::ASNEnemy(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	EnemyAbilitySystemComponent = CreateDefaultSubobject<USNAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	EnemyAbilitySystemComponent->SetIsReplicated(true);
	EnemyAbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);
	
	AbilitySystemComponent = Cast<USNAbilitySystemComponent>(EnemyAbilitySystemComponent);
	
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
		
		AbilitySet->GiveToAbilitySystem(Cast<USNAbilitySystemComponent>(AbilitySystemComponent), nullptr, nullptr);
	
		AttributesComponent->InitializeWithAbilitySystem(Cast<USNAbilitySystemComponent>(AbilitySystemComponent));
	}
	
	HealthChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		AttributesComponent->GetHealthAttributes()).AddUObject(this, &ThisClass::HealthChanged);
		
}

void ASNEnemy::HealthChanged(const FOnAttributeChangeData& Data)
{
	UE_LOG(LogTemp, Warning, TEXT("Attribute: %f"), AttributesComponent->GetHealth());
}
