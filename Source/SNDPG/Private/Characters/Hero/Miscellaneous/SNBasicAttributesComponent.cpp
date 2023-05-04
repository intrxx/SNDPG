// Copyright 2023 Michal Oginski.


#include "Characters/Hero/Miscellaneous/SNBasicAttributesComponent.h"
#include "GAS/Attributes/SNBasicAttributes.h"
#include "GAS/SNAbilitySystemComponent.h"

// Sets default values for this component's properties
USNBasicAttributesComponent::USNBasicAttributesComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bStartWithTickEnabled = false;
	PrimaryComponentTick.bCanEverTick = false;
	
	SetIsReplicatedByDefault(true);
	
	AbilitySystemComponent = nullptr;
	BasicAttributes = nullptr;
	DeathState = ESNDeathState::NotDead;
}

void USNBasicAttributesComponent::InitializeWithAbilitySystem(USNAbilitySystemComponent* InASC)
{
	AActor* Owner = GetOwner();
	check(Owner);

	if (AbilitySystemComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("SNBasicAttributesComponent: Component for owner [%s] has already been initialized with an ability system."), *GetNameSafe(Owner));
		return;
	}

	AbilitySystemComponent = InASC;
	if (!AbilitySystemComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("SNBasicAttributesComponent: Cannot initialize component for owner [%s] with NULL ability system."), *GetNameSafe(Owner));
		return;
	}

	BasicAttributes = AbilitySystemComponent->GetSet<USNBasicAttributes>();
	if (!BasicAttributes)
	{
		UE_LOG(LogTemp, Error, TEXT("SNBasicAttributesComponent: Cannot initialize  omponent for owner [%s] with NULL attribute set on the ability system."), *GetNameSafe(Owner));
		return;
	}

	//TODO Listen for atribute changes
	
	// TEMP: Reset attributes to default values.  Eventually this will be driven by a spread sheet.
	AbilitySystemComponent->SetNumericAttributeBase(USNBasicAttributes::GetHealthAttribute(), BasicAttributes->GetMaxHealth());
}

void USNBasicAttributesComponent::UninitializeFromAbilitySystem()
{
	BasicAttributes = nullptr;
	AbilitySystemComponent = nullptr;
}

float USNBasicAttributesComponent::GetHealth() const
{
	return (BasicAttributes ? BasicAttributes->GetHealth() : 0.0f);
}

float USNBasicAttributesComponent::GetMaxHealth() const
{
	return (BasicAttributes ? BasicAttributes->GetMaxHealth() : 0.0f);
}


// Called when the game starts
void USNBasicAttributesComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

