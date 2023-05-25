// Copyright 2023 Michal Oginski.


#include "Characters/Hero/Miscellaneous/SNBasicAttributesComponent.h"

#include "Characters/Enemy/SNEnemy.h"
#include "Characters/Hero/SNHero.h"
#include "Characters/Hero/Miscellaneous/SNHeroController.h"
#include "GameplayTags/SNGameplayTags.h"
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
	
	HealthChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		GetHealthAttribute()).AddUObject(this, &ThisClass::HealthChanged);
	MaxHealthChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		GetMaxHealthAttribute()).AddUObject(this, &ThisClass::MaxHealthChanged);
	ResourceChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		GetResourceAttribute()).AddUObject(this, &ThisClass::ResourceChanged);
	MaxResourceChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		GetMaxResourceAttribute()).AddUObject(this, &ThisClass::MaxResourceChanged);
	CharacterLevelChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		GetCharacterLevelAttribute()).AddUObject(this, &ThisClass::CharacterLevelChanged);
	ExperienceChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		GetExperienceAttribute()).AddUObject(this, &ThisClass::ExperienceChanged);
	MaxExperienceChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		GetMaxExperienceAttribute()).AddUObject(this, &ThisClass::MaxExperienceChanged);
	ArmourChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		GetArmourAttribute()).AddUObject(this, &ThisClass::ArmourChanged);
	StrengthChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		GetStrengthAttribute()).AddUObject(this, &ThisClass::StrengthChanged);
	EnduranceChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		GetEnduranceAttribute()).AddUObject(this, &ThisClass::EnduranceChanged);
	FaithChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		GetFaithAttribute()).AddUObject(this, &ThisClass::FaithChanged);
	HealingChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		GetHealingAttribute()).AddUObject(this, &ThisClass::HealingChanged);
	LevelUpPointsChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		GetLevelUpPointsAttribute()).AddUObject(this, &ThisClass::LevelUpPointsChanged);
	VitalityChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		GetVitalityAttribute()).AddUObject(this, &ThisClass::Vitalitychanged);

	BasicAttributes->OnOutOfHealth.AddUObject(this, &ThisClass::HandleOutOfHealth);
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

FGameplayAttribute USNBasicAttributesComponent::GetHealthAttribute() const
{
	return (BasicAttributes ? BasicAttributes->GetHealthAttribute() : nullptr);
}

FGameplayAttribute USNBasicAttributesComponent::GetMaxHealthAttribute() const
{
	return (BasicAttributes ? BasicAttributes->GetMaxHealthAttribute() : nullptr);
}

FGameplayAttribute USNBasicAttributesComponent::GetResourceAttribute() const
{
	return (BasicAttributes ? BasicAttributes->GetResourceAttribute() : nullptr);
}

FGameplayAttribute USNBasicAttributesComponent::GetMaxResourceAttribute() const
{
	return (BasicAttributes ? BasicAttributes->GetMaxResourceAttribute() : nullptr);
}

FGameplayAttribute USNBasicAttributesComponent::GetCharacterLevelAttribute() const
{
	return (BasicAttributes ? BasicAttributes->GetCharacterLevelAttribute() : nullptr);
}

FGameplayAttribute USNBasicAttributesComponent::GetExperienceAttribute() const
{
	return (BasicAttributes ? BasicAttributes->GetExperienceAttribute() : nullptr);
}

FGameplayAttribute USNBasicAttributesComponent::GetMaxExperienceAttribute() const
{
	return (BasicAttributes ? BasicAttributes->GetMaxExperienceAttribute() : nullptr);
}

FGameplayAttribute USNBasicAttributesComponent::GetArmourAttribute() const
{
	return (BasicAttributes ? BasicAttributes->GetArmourAttribute() : nullptr);
}

FGameplayAttribute USNBasicAttributesComponent::GetStrengthAttribute() const
{
	return (BasicAttributes ? BasicAttributes->GetStrengthAttribute() : nullptr);
}

FGameplayAttribute USNBasicAttributesComponent::GetEnduranceAttribute() const
{
	return (BasicAttributes ? BasicAttributes->GetEnduranceAttribute() : nullptr);
}

FGameplayAttribute USNBasicAttributesComponent::GetFaithAttribute() const
{
	return (BasicAttributes ? BasicAttributes->GetFaithAttribute() : nullptr);
}

FGameplayAttribute USNBasicAttributesComponent::GetHealingAttribute() const
{
	return (BasicAttributes ? BasicAttributes->GetHealingAttribute() : nullptr);
}

FGameplayAttribute USNBasicAttributesComponent::GetLevelUpPointsAttribute() const
{
	return (BasicAttributes ? BasicAttributes->GetLevelUpPointsAttribute() : nullptr);
}

FGameplayAttribute USNBasicAttributesComponent::GetVitalityAttribute() const
{
	return (BasicAttributes ? BasicAttributes->GetVitalityAttribute() : nullptr);
}

float USNBasicAttributesComponent::GetVitality() const
{
	return (BasicAttributes ? BasicAttributes->GetVitality() : 0.0f);
}

float USNBasicAttributesComponent::GetLevelUpPoints() const
{
	return (BasicAttributes ? BasicAttributes->GetLevelUpPoints() : 0.0f);
}

float USNBasicAttributesComponent::GetResource() const
{
	return (BasicAttributes ? BasicAttributes->GetResource() : 0.0f);
}

float USNBasicAttributesComponent::GetMaxResource() const
{
	return (BasicAttributes ? BasicAttributes->GetMaxResource() : 0.0f);
}

float USNBasicAttributesComponent::GetCharacterLevel() const
{
	return (BasicAttributes ? BasicAttributes->GetCharacterLevel() : 0.0f);
}

float USNBasicAttributesComponent::GetExperience() const
{
	return (BasicAttributes ? BasicAttributes->GetExperience() : 0.0f);
}

float USNBasicAttributesComponent::GetMaxExperience() const
{
	return (BasicAttributes ? BasicAttributes->GetMaxExperience() : 0.0f);
}

float USNBasicAttributesComponent::GetArmour() const
{
	return (BasicAttributes ? BasicAttributes->GetArmour() : 0.0f);
}

float USNBasicAttributesComponent::GetStrength() const
{
	return (BasicAttributes ? BasicAttributes->GetStrength() : 0.0f);
}

float USNBasicAttributesComponent::GetEndurance() const
{
	return (BasicAttributes ? BasicAttributes->GetEndurance() : 0.0f);
}

float USNBasicAttributesComponent::GetFaith() const
{
	return (BasicAttributes ? BasicAttributes->GetFaith() : 0.0f);
}

float USNBasicAttributesComponent::GetHealing() const
{
	return (BasicAttributes ? BasicAttributes->GetHealing() : 0.0f);
}

// Called when the game starts
void USNBasicAttributesComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void USNBasicAttributesComponent::HandleOutOfHealth(AActor* DamageInstigator, AActor* DamageCauser,
	const FGameplayEffectSpec& DamageEffectSpec, float DamageMagnitude)
{
#if WITH_SERVER_CODE
	
	if (AbilitySystemComponent)
	{
		// Send the "GameplayEvent.Death" gameplay event through the owner's ability system.  This can be used to trigger a death gameplay ability.
		{
			FGameplayEventData Payload;
			Payload.EventTag = FSNGameplayTags::Get().GameplayEvent_Death;
			Payload.Instigator = DamageInstigator;
			Payload.Target = AbilitySystemComponent->GetAvatarActor();
			Payload.OptionalObject = DamageEffectSpec.Def;
			Payload.ContextHandle = DamageEffectSpec.GetEffectContext();
			Payload.InstigatorTags = *DamageEffectSpec.CapturedSourceTags.GetAggregatedTags();
			Payload.TargetTags = *DamageEffectSpec.CapturedTargetTags.GetAggregatedTags();
			Payload.EventMagnitude = DamageMagnitude;

			FScopedPredictionWindow NewScopedWindow(AbilitySystemComponent, true);
			AbilitySystemComponent->HandleGameplayEvent(Payload.EventTag, &Payload);
		}
	}
#endif
}

void USNBasicAttributesComponent::StartDeath()
{
	if (DeathState != ESNDeathState::NotDead)
	{
		return;
	}
	
	DeathState = ESNDeathState::DeathStarted;

	AActor* Owner = GetOwner();
	check(Owner);

	OnDeathStarted.Broadcast(Owner);
}

void USNBasicAttributesComponent::FinishDeath()
{
	if(DeathState != ESNDeathState::DeathStarted)
	{
		return;
	}

	DeathState = ESNDeathState::DeathFinished;

	AActor* Owner = GetOwner();
	check(Owner);

	OnDeathFinished.Broadcast(Owner);
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
				HeroHUD->SetMaxResource(MaxResource);
				HeroHUD->SetResourcePercentage(GetResource() / MaxResource);
			}
		}
	}
}

void USNBasicAttributesComponent::ExperienceChanged(const FOnAttributeChangeData& Data)
{
	float Experience = Data.NewValue;
	
	if(ASNHero* HeroOwner = Cast<ASNHero>(GetOwner()))
	{
		ASNHeroController* PC = Cast<ASNHeroController>(HeroOwner->GetController());
		if(PC)
		{
			USNHeroHUD* HeroHUD = Cast<USNHeroHUD>(PC->GetHeroHUD());
			if(HeroHUD)
			{
				HeroHUD->SetExperience(Experience);
			}
		}
	}
}

void USNBasicAttributesComponent::MaxExperienceChanged(const FOnAttributeChangeData& Data)
{
	float MaxExperience = Data.NewValue;
	
	if(ASNHero* HeroOwner = Cast<ASNHero>(GetOwner()))
	{
		ASNHeroController* PC = Cast<ASNHeroController>(HeroOwner->GetController());
		if(PC)
		{
			USNHeroHUD* HeroHUD = Cast<USNHeroHUD>(PC->GetHeroHUD());
			if(HeroHUD)
			{
				HeroHUD->SetMaxExperience(MaxExperience);
			}
		}
	}
}

void USNBasicAttributesComponent::CharacterLevelChanged(const FOnAttributeChangeData& Data)
{
	float CharacterLevel = Data.NewValue;
	
	if(ASNHero* HeroOwner = Cast<ASNHero>(GetOwner()))
	{
		ASNHeroController* PC = Cast<ASNHeroController>(HeroOwner->GetController());
		if(PC)
		{
			USNHeroHUD* HeroHUD = Cast<USNHeroHUD>(PC->GetHeroHUD());
			if(HeroHUD)
			{
				HeroHUD->SetCharacterLevel(CharacterLevel);
			}
		}
	}
}

void USNBasicAttributesComponent::ArmourChanged(const FOnAttributeChangeData& Data)
{
	float Armour = Data.NewValue;
	
	if(ASNHero* HeroOwner = Cast<ASNHero>(GetOwner()))
	{
		ASNHeroController* PC = Cast<ASNHeroController>(HeroOwner->GetController());
		if(PC)
		{
			USNHeroHUD* HeroHUD = Cast<USNHeroHUD>(PC->GetHeroHUD());
			if(HeroHUD)
			{
				HeroHUD->SetArmour(Armour);
			}
		}
	}
}

void USNBasicAttributesComponent::StrengthChanged(const FOnAttributeChangeData& Data)
{
	float Strength = Data.NewValue;
	
	if(ASNHero* HeroOwner = Cast<ASNHero>(GetOwner()))
	{
		ASNHeroController* PC = Cast<ASNHeroController>(HeroOwner->GetController());
		if(PC)
		{
			USNHeroHUD* HeroHUD = Cast<USNHeroHUD>(PC->GetHeroHUD());
			if(HeroHUD)
			{
				HeroHUD->SetStrength(Strength);
			}
		}
	}
}

void USNBasicAttributesComponent::EnduranceChanged(const FOnAttributeChangeData& Data)
{
	float Endurance = Data.NewValue;
	
	if(ASNHero* HeroOwner = Cast<ASNHero>(GetOwner()))
	{
		ASNHeroController* PC = Cast<ASNHeroController>(HeroOwner->GetController());
		if(PC)
		{
			USNHeroHUD* HeroHUD = Cast<USNHeroHUD>(PC->GetHeroHUD());
			if(HeroHUD)
			{
				HeroHUD->SetEndurance(Endurance);
			}
		}
	}
}

void USNBasicAttributesComponent::FaithChanged(const FOnAttributeChangeData& Data)
{
	float Faith = Data.NewValue;
	
	if(ASNHero* HeroOwner = Cast<ASNHero>(GetOwner()))
	{
		ASNHeroController* PC = Cast<ASNHeroController>(HeroOwner->GetController());
		if(PC)
		{
			USNHeroHUD* HeroHUD = Cast<USNHeroHUD>(PC->GetHeroHUD());
			if(HeroHUD)
			{
				HeroHUD->SetFaith(Faith);
				HeroHUD->SetHealingRange(GetHealing(), Faith);
			}
		}
	}
}

void USNBasicAttributesComponent::HealingChanged(const FOnAttributeChangeData& Data)
{
	float Healing = Data.NewValue;
	
	if(ASNHero* HeroOwner = Cast<ASNHero>(GetOwner()))
	{
		ASNHeroController* PC = Cast<ASNHeroController>(HeroOwner->GetController());
		if(PC)
		{
			USNHeroHUD* HeroHUD = Cast<USNHeroHUD>(PC->GetHeroHUD());
			if(HeroHUD)
			{
				HeroHUD->SetHealingRange(Healing, GetFaith());
			}
		}
	}
}

void USNBasicAttributesComponent::LevelUpPointsChanged(const FOnAttributeChangeData& Data)
{
	float LevelUpPoints = Data.NewValue;
	
	if(ASNHero* HeroOwner = Cast<ASNHero>(GetOwner()))
	{
		ASNHeroController* PC = Cast<ASNHeroController>(HeroOwner->GetController());
		if(PC)
		{
			USNHeroHUD* HeroHUD = Cast<USNHeroHUD>(PC->GetHeroHUD());
			if(HeroHUD)
			{
				HeroHUD->SetLevelUpPoints(LevelUpPoints);
			}
		}
	}
}

void USNBasicAttributesComponent::Vitalitychanged(const FOnAttributeChangeData& Data)
{
	float Vitality = Data.NewValue;
	
	if(ASNHero* HeroOwner = Cast<ASNHero>(GetOwner()))
	{
		ASNHeroController* PC = Cast<ASNHeroController>(HeroOwner->GetController());
		if(PC)
		{
			USNHeroHUD* HeroHUD = Cast<USNHeroHUD>(PC->GetHeroHUD());
			if(HeroHUD)
			{
				HeroHUD->SetVitality(Vitality);
			}
		}
	}
}



