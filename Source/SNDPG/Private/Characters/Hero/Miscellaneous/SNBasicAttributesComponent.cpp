// Copyright 2023 Michal Oginski.


#include "Characters/Hero/Miscellaneous/SNBasicAttributesComponent.h"

#include "Characters/Enemy/SNEnemy.h"
#include "Characters/Hero/SNHero.h"
#include "Characters/Hero/Miscellaneous/SNHeroController.h"
#include "GAS/Attributes/SNBasicAttributes.h"
#include "GAS/SNAbilitySystemComponent.h"
#include "UI/SNHealthBarWidget.h"
#include "UI/SNHeroHUD.h"

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
	HealthChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		BasicAttributes->GetHealthAttribute()).AddUObject(this, &ThisClass::HealthChanged);
	MaxHealthChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		BasicAttributes->GetMaxHealthAttribute()).AddUObject(this, &ThisClass::MaxHealthChanged);
	ResourceChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		BasicAttributes->GetResourceAttribute()).AddUObject(this, &ThisClass::ResourceChanged);
	MaxResourceChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		BasicAttributes->GetMaxResourceAttribute()).AddUObject(this, &ThisClass::MaxResourceChanged);
	
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

FGameplayAttribute USNBasicAttributesComponent::GetHealthAttributes() const
{
	return BasicAttributes->GetHealthAttribute();
}

float USNBasicAttributesComponent::GetResource() const
{
	return (BasicAttributes ? BasicAttributes->GetResource() : 0.0f);
}

float USNBasicAttributesComponent::GetMaxResource() const
{
	return (BasicAttributes ? BasicAttributes->GetMaxResource() : 0.0f);
}

// Called when the game starts
void USNBasicAttributesComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void USNBasicAttributesComponent::HealthChanged(const FOnAttributeChangeData& Data)
{
	float Health = Data.NewValue;
	if(ASNEnemy* EnemyOwner = Cast<ASNEnemy>(GetOwner()))
	{
		USNHealthBarWidget* HealthBarWidget = EnemyOwner->GetHealthBarWidget();
		if(HealthBarWidget)
		{
			HealthBarWidget->SetHealthPercentage(Health / GetMaxHealth());
		}
	}
	else if(ASNHero* HeroOwner = Cast<ASNHero>(GetOwner()))
	{
		ASNHeroController* PC = Cast<ASNHeroController>(HeroOwner->GetController());
		if(PC)
		{
			USNHeroHUD* HeroHUD = Cast<USNHeroHUD>(PC->GetHeroHUD());
			if(HeroHUD)
			{
				HeroHUD->SetHealth(Health);
				HeroHUD->SetHealthPercentage(Health / GetMaxHealth());
			}
		}
	}
}

void USNBasicAttributesComponent::MaxHealthChanged(const FOnAttributeChangeData& Data)
{
	float MaxHealth = Data.NewValue;
	if(ASNEnemy* EnemyOwner = Cast<ASNEnemy>(GetOwner()))
	{
		USNHealthBarWidget* HealthBarWidget = EnemyOwner->GetHealthBarWidget();
		if(HealthBarWidget)
		{
			HealthBarWidget->SetHealthPercentage(GetHealth() / MaxHealth);
		}
	}
	else if(ASNHero* HeroOwner = Cast<ASNHero>(GetOwner()))
	{
		ASNHeroController* PC = Cast<ASNHeroController>(HeroOwner->GetController());
		if(PC)
		{
			USNHeroHUD* HeroHUD = Cast<USNHeroHUD>(PC->GetHeroHUD());
			if(HeroHUD)
			{
				HeroHUD->SetMaxHealth(MaxHealth);
				HeroHUD->SetHealthPercentage(GetHealth() / MaxHealth);
			}
		}
	}
}

void USNBasicAttributesComponent::ResourceChanged(const FOnAttributeChangeData& Data)
{
	float Resource = Data.NewValue;
	if(ASNHero* HeroOwner = Cast<ASNHero>(GetOwner()))
	{
		ASNHeroController* PC = Cast<ASNHeroController>(HeroOwner->GetController());
		if(PC)
		{
			USNHeroHUD* HeroHUD = Cast<USNHeroHUD>(PC->GetHeroHUD());
			if(HeroHUD)
			{
				HeroHUD->SetResource(Resource);
				HeroHUD->SetResourcePercentage(Resource / GetMaxResource());
			}
		}
	}
}

void USNBasicAttributesComponent::MaxResourceChanged(const FOnAttributeChangeData& Data)
{
	float MaxResource = Data.NewValue;
	if(ASNHero* HeroOwner = Cast<ASNHero>(GetOwner()))
	{
		ASNHeroController* PC = Cast<ASNHeroController>(HeroOwner->GetController());
		if(PC)
		{
			USNHeroHUD* HeroHUD = Cast<USNHeroHUD>(PC->GetHeroHUD());
			if(HeroHUD)
			{
				HeroHUD->SetResource(MaxResource);
				HeroHUD->SetResourcePercentage(GetResource() / MaxResource);
			}
		}
	}
}

