// Copyright 2023 Michal Oginski.


#include "ActorComponents/SNBasicAttributesComponent.h"
#include "Characters/Enemy/SNEnemy.h"
#include "Characters/Hero/SNHero.h"
#include "Characters/Hero/Miscellaneous/SNHeroController.h"
#include "GameplayTags/SNGameplayTags.h"
#include "GAS/Attributes/SNBasicAttributes.h"
#include "GAS/SNAbilitySystemComponent.h"
#include "UI/HeroHUD/SNCharacterStatusWidget.h"
#include "UI/EnemyUI/SNHealthBarWidget.h"
#include "UI/HeroHUD/SNHeroHUD.h"
#include "UI/Inventory/SNEquipmentWidget.h"
#include "UI/Inventory/SNInventoryWidget.h"

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
	StaminaChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		GetStaminaAttribute()).AddUObject(this, &ThisClass::StaminaChanged);
	MaxStaminaChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		GetMaxStaminaAttribute()).AddUObject(this, &ThisClass::MaxStaminaChanged);
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
		GetVitalityAttribute()).AddUObject(this, &ThisClass::VitalityChanged);
	ArcaneChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		GetArcaneAttribute()).AddUObject(this, &ThisClass::ArcaneChanged);
	MindChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		GetMindAttribute()).AddUObject(this, &ThisClass::MindChanged);
	ReplenishingChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		GetReplenishingAttribute()).AddUObject(this, &ThisClass::ReplenishingChanged);
	GoldChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		GetGoldAttribute()).AddUObject(this, &ThisClass::GoldChanged);

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

float USNBasicAttributesComponent::GetStamina() const
{
	return (BasicAttributes ? BasicAttributes->GetStamina() : 0.0f);
}

float USNBasicAttributesComponent::GetMaxStamina() const
{
	return (BasicAttributes ? BasicAttributes->GetMaxStamina() : 0.0f);
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

float USNBasicAttributesComponent::GetReplenishing() const
{
	return (BasicAttributes ? BasicAttributes->GetReplenishing() : 0.0f);
}

float USNBasicAttributesComponent::GetArcane() const
{
	return (BasicAttributes ? BasicAttributes->GetArcane() : 0.0f);
}

float USNBasicAttributesComponent::GetMind() const
{
	return (BasicAttributes ? BasicAttributes->GetMind() : 0.0f);
}

float USNBasicAttributesComponent::GetGold() const
{
	return (BasicAttributes ? BasicAttributes->GetGold() : 0.0f);
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

FGameplayAttribute USNBasicAttributesComponent::GetReplenishingAttribute() const
{
	return (BasicAttributes ? BasicAttributes->GetReplenishingAttribute() : nullptr);
}

FGameplayAttribute USNBasicAttributesComponent::GetLevelUpPointsAttribute() const
{
	return (BasicAttributes ? BasicAttributes->GetLevelUpPointsAttribute() : nullptr);
}

FGameplayAttribute USNBasicAttributesComponent::GetVitalityAttribute() const
{
	return (BasicAttributes ? BasicAttributes->GetVitalityAttribute() : nullptr);
}

FGameplayAttribute USNBasicAttributesComponent::GetStaminaAttribute() const
{
	return (BasicAttributes ? BasicAttributes->GetStaminaAttribute() : nullptr);
}

FGameplayAttribute USNBasicAttributesComponent::GetMaxStaminaAttribute() const
{
	return (BasicAttributes ? BasicAttributes->GetMaxStaminaAttribute() : nullptr);
}

FGameplayAttribute USNBasicAttributesComponent::GetArcaneAttribute() const
{
	return (BasicAttributes ? BasicAttributes->GetArcaneAttribute() : nullptr);
}
FGameplayAttribute USNBasicAttributesComponent::GetMindAttribute() const
{
	return (BasicAttributes ? BasicAttributes->GetMindAttribute() : nullptr);
}

FGameplayAttribute USNBasicAttributesComponent::GetGoldAttribute() const
{
	return (BasicAttributes ? BasicAttributes->GetGoldAttribute() : nullptr);
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

void USNBasicAttributesComponent::LevelUpStrength(float Amount)
{
	SubtractLevelUpPoints(-Amount);
	AddLeveledUpAttribute(Amount, GetStrengthAttribute());
}

void USNBasicAttributesComponent::LevelUpArcane(float Amount)
{
	SubtractLevelUpPoints(-Amount);
	AddLeveledUpAttribute(Amount, GetArcaneAttribute());
}

void USNBasicAttributesComponent::LevelUpFaith(float Amount)
{
	SubtractLevelUpPoints(-Amount);
	AddLeveledUpAttribute(Amount, GetFaithAttribute());
}

void USNBasicAttributesComponent::LevelUpVitality(float Amount)
{
	SubtractLevelUpPoints(-Amount);
	AddLeveledUpAttribute(Amount, GetVitalityAttribute());

	// Increase Health by 5 for every added point to Vitality
	AddLeveledUpAttribute(FMath::RoundToFloat((GetMaxHealth() / 50.0f) + Amount * 5.0f), GetMaxHealthAttribute());
	AddLeveledUpAttribute(GetMaxHealth() - GetHealth(), GetHealthAttribute());
}

void USNBasicAttributesComponent::LevelUpEndurance(float Amount)
{
	SubtractLevelUpPoints(-Amount);
	AddLeveledUpAttribute(Amount, GetEnduranceAttribute());

	// Increase Stamina based on endurance
	AddLeveledUpAttribute(FMath::RoundToFloat((GetStamina() / 75.0f) + Amount * 3.0f), GetMaxStaminaAttribute());
	AddLeveledUpAttribute(GetMaxStamina() - GetStamina(), GetStaminaAttribute());
}

void USNBasicAttributesComponent::LevelUpMind(float Amount)
{
	SubtractLevelUpPoints(-Amount);
	AddLeveledUpAttribute(Amount, GetMindAttribute());

	// Increase mana based on mind
	AddLeveledUpAttribute(FMath::RoundToFloat((GetResource() / 75.0f) + Amount * 5.0f), GetMaxResourceAttribute());
	AddLeveledUpAttribute(GetMaxResource() - GetResource(), GetResourceAttribute());
}

void USNBasicAttributesComponent::SubtractLevelUpPoints(float Amount)
{
	// Create a dynamic instant Gameplay Effect that removes Level Up Points after clicking the level up button
	UGameplayEffect* RemoveLevelUpPoint = NewObject<UGameplayEffect>(GetTransientPackage(), FName(TEXT("EarningExperience")));
	RemoveLevelUpPoint->DurationPolicy = EGameplayEffectDurationType::Instant;
						
	RemoveLevelUpPoint->Modifiers.SetNum(1);
	FGameplayModifierInfo& InfoXP = RemoveLevelUpPoint->Modifiers[0];
	InfoXP.ModifierMagnitude = FScalableFloat(Amount);
	InfoXP.ModifierOp = EGameplayModOp::Additive;
	InfoXP.Attribute = USNBasicAttributes::GetLevelUpPointsAttribute();
						
	AbilitySystemComponent->ApplyGameplayEffectToSelf(RemoveLevelUpPoint, 1.0f, AbilitySystemComponent->MakeEffectContext());
}

void USNBasicAttributesComponent::AddLeveledUpAttribute(float Amount, FGameplayAttribute AttributeToAdd)
{
	// Create a dynamic instant Gameplay Effect that increases Attribute
	UGameplayEffect* IncreaseAttribute = NewObject<UGameplayEffect>(GetTransientPackage(), FName(TEXT("EarningExperience")));
	IncreaseAttribute->DurationPolicy = EGameplayEffectDurationType::Instant;
						
	IncreaseAttribute->Modifiers.SetNum(1);
	FGameplayModifierInfo& InfoXP = IncreaseAttribute->Modifiers[0];
	InfoXP.ModifierMagnitude = FScalableFloat(Amount);
	InfoXP.ModifierOp = EGameplayModOp::Additive;
	InfoXP.Attribute = AttributeToAdd;
						
	AbilitySystemComponent->ApplyGameplayEffectToSelf(IncreaseAttribute, 1.0f, AbilitySystemComponent->MakeEffectContext());
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
				HeroHUD->SetHealthPercentage(Health / GetMaxHealth());
			}

			USNCharacterStatusWidget* Status = Cast<USNCharacterStatusWidget>(PC->GetCharacterStatusUI());
			if(Status)
			{
				Status->SetHealth(Health, GetMaxHealth());
			}
			
			USNInventoryWidget* Inventory = Cast<USNInventoryWidget>(PC->GetInventoryUI());
			if(Inventory)
			{
				Inventory->SetHealth(Health, GetMaxHealth());
			}

			USNEquipmentWidget* Equipment = Cast<USNEquipmentWidget>(PC->GetEquipmentUI());
			if(Equipment)
			{
				Equipment->SetHealth(Health, GetMaxHealth());
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

			USNCharacterStatusWidget* Status = Cast<USNCharacterStatusWidget>(PC->GetCharacterStatusUI());
			if(Status)
			{
				Status->SetHealth(GetHealth(), MaxHealth);
			}

			USNInventoryWidget* Inventory = Cast<USNInventoryWidget>(PC->GetInventoryUI());
			if(Inventory)
			{
				Inventory->SetHealth(GetHealth(), MaxHealth);
			}

			USNEquipmentWidget* Equipment = Cast<USNEquipmentWidget>(PC->GetEquipmentUI());
			if(Equipment)
			{
				Equipment->SetHealth(GetHealth(), MaxHealth);
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

			USNCharacterStatusWidget* Status = Cast<USNCharacterStatusWidget>(PC->GetCharacterStatusUI());
			if(Status)
			{
				Status->SetResource(Resource, GetMaxResource());
			}
			
			USNInventoryWidget* Inventory = Cast<USNInventoryWidget>(PC->GetInventoryUI());
			if(Inventory)
			{
				Inventory->SetResource(Resource, GetMaxResource());
			}

			USNEquipmentWidget* Equipment = Cast<USNEquipmentWidget>(PC->GetEquipmentUI());
			if(Equipment)
			{
				Equipment->SetResource(Resource, GetMaxResource());
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

			USNCharacterStatusWidget* Status = Cast<USNCharacterStatusWidget>(PC->GetCharacterStatusUI());
			if(Status)
			{
				Status->SetResource(GetResource(), MaxResource);
			}

			USNInventoryWidget* Inventory = Cast<USNInventoryWidget>(PC->GetInventoryUI());
			if(Inventory)
			{
				Inventory->SetResource(GetResource(), MaxResource);
			}

			USNEquipmentWidget* Equipment = Cast<USNEquipmentWidget>(PC->GetEquipmentUI());
			if(Equipment)
			{
				Equipment->SetResource(GetResource(), MaxResource);
			}
		}
	}
}

void USNBasicAttributesComponent::StaminaChanged(const FOnAttributeChangeData& Data)
{
	float Stamina = Data.NewValue;
	
	if(ASNHero* HeroOwner = Cast<ASNHero>(GetOwner()))
	{
		ASNHeroController* PC = Cast<ASNHeroController>(HeroOwner->GetController());
		if(PC)
		{
			USNHeroHUD* HeroHUD = Cast<USNHeroHUD>(PC->GetHeroHUD());
			if(HeroHUD)
			{
				HeroHUD->SetStaminaPercentage(Stamina / GetMaxStamina());
			}
		}
	}
}

void USNBasicAttributesComponent::MaxStaminaChanged(const FOnAttributeChangeData& Data)
{
	float MaxStamina = Data.NewValue;
	
	if(ASNHero* HeroOwner = Cast<ASNHero>(GetOwner()))
	{
		ASNHeroController* PC = Cast<ASNHeroController>(HeroOwner->GetController());
		if(PC)
		{
			USNHeroHUD* HeroHUD = Cast<USNHeroHUD>(PC->GetHeroHUD());
			if(HeroHUD)
			{
				HeroHUD->SetMaxStamina(MaxStamina);
				HeroHUD->SetStaminaPercentage(GetStamina() / MaxStamina);
			}

			USNCharacterStatusWidget* Status = Cast<USNCharacterStatusWidget>(PC->GetCharacterStatusUI());
			if(Status)
			{
				Status->SetMaxStamina(MaxStamina);
			}

			USNInventoryWidget* Inventory = Cast<USNInventoryWidget>(PC->GetInventoryUI());
			if(Inventory)
			{
				Inventory->SetMaxStamina(MaxStamina);
			}

			USNEquipmentWidget* Equipment = Cast<USNEquipmentWidget>(PC->GetEquipmentUI());
			if(Equipment)
			{
				Equipment->SetMaxStamina(MaxStamina);
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
			USNCharacterStatusWidget* Status = Cast<USNCharacterStatusWidget>(PC->GetCharacterStatusUI());
			if(Status)
			{
				Status->SetExperience(Experience);
			}

			USNInventoryWidget* Inventory = Cast<USNInventoryWidget>(PC->GetInventoryUI());
			if(Inventory)
			{
				Inventory->SetExperience(Experience);
			}

			USNEquipmentWidget* Equipment = Cast<USNEquipmentWidget>(PC->GetEquipmentUI());
			if(Equipment)
			{
				Equipment->SetExperience(Experience);
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
			USNCharacterStatusWidget* Status = Cast<USNCharacterStatusWidget>(PC->GetCharacterStatusUI());
			if(Status)
			{
				Status->SetMaxExperience(MaxExperience);
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
			USNCharacterStatusWidget* Status = Cast<USNCharacterStatusWidget>(PC->GetCharacterStatusUI());
			if(Status)
			{
				Status->SetCharacterLevel(CharacterLevel);
			}

			USNInventoryWidget* Inventory = Cast<USNInventoryWidget>(PC->GetInventoryUI());
			if(Inventory)
			{
				Inventory->SetCharacterLevel(CharacterLevel);
			}
			
			USNEquipmentWidget* Equipment = Cast<USNEquipmentWidget>(PC->GetEquipmentUI());
			if(Equipment)
			{
				Equipment->SetCharacterLevel(CharacterLevel);
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
			USNCharacterStatusWidget* Status = Cast<USNCharacterStatusWidget>(PC->GetCharacterStatusUI());
			if(Status)
			{
				Status->SetArmour(Armour);
			}
			
			USNInventoryWidget* Inventory = Cast<USNInventoryWidget>(PC->GetInventoryUI());
			if(Inventory)
			{
				Inventory->SetArmour(Armour);
			}

			USNEquipmentWidget* Equipment = Cast<USNEquipmentWidget>(PC->GetEquipmentUI());
			if(Equipment)
			{
				Equipment->SetArmour(Armour);
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
			USNCharacterStatusWidget* Status = Cast<USNCharacterStatusWidget>(PC->GetCharacterStatusUI());
			if(Status)
			{
				Status->SetStrength(Strength);
				Status->SetR1Range(HeroOwner->R1BaseDamage, Strength);
				Status->SetR2Range(HeroOwner->R2BaseDamage, Strength);
				Status->SetL1Range(HeroOwner->L1BaseDamage, Strength);
			}

			USNInventoryWidget* Inventory = Cast<USNInventoryWidget>(PC->GetInventoryUI());
			if(Inventory)
			{
				Inventory->SetStrength(Strength);
			}

			USNEquipmentWidget* Equipment = Cast<USNEquipmentWidget>(PC->GetEquipmentUI());
			if(Equipment)
			{
				Equipment->SetStrength(Strength);
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
			USNCharacterStatusWidget* Status = Cast<USNCharacterStatusWidget>(PC->GetCharacterStatusUI());
			if(Status)
			{
				Status->SetEndurance(Endurance);
			}

			USNInventoryWidget* Inventory = Cast<USNInventoryWidget>(PC->GetInventoryUI());
			if(Inventory)
			{
				Inventory->SetEndurance(Endurance);
			}
			
			USNEquipmentWidget* Equipment = Cast<USNEquipmentWidget>(PC->GetEquipmentUI());
			if(Equipment)
			{
				Equipment->SetEndurance(Endurance);
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
			USNCharacterStatusWidget* Status = Cast<USNCharacterStatusWidget>(PC->GetCharacterStatusUI());
			if(Status)
			{
				Status->SetFaith(Faith);
				Status->SetHealingRange(GetHealing(), Faith);
				Status->SetReplenishingRange(GetReplenishing(), Faith);
			}

			USNInventoryWidget* Inventory = Cast<USNInventoryWidget>(PC->GetInventoryUI());
			if(Inventory)
			{
				Inventory->SetFaith(Faith);
			}

			USNEquipmentWidget* Equipment = Cast<USNEquipmentWidget>(PC->GetEquipmentUI());
			if(Equipment)
			{
				Equipment->SetFaith(Faith);
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
			USNCharacterStatusWidget* Status = Cast<USNCharacterStatusWidget>(PC->GetCharacterStatusUI());
			if(Status)
			{
				Status->SetHealingRange(Healing, GetFaith());
			}
		}
	}
}

void USNBasicAttributesComponent::ReplenishingChanged(const FOnAttributeChangeData& Data)
{
	float Replenishing = Data.NewValue;
	
	if(ASNHero* HeroOwner = Cast<ASNHero>(GetOwner()))
	{
		ASNHeroController* PC = Cast<ASNHeroController>(HeroOwner->GetController());
		if(PC)
		{
			USNCharacterStatusWidget* Status = Cast<USNCharacterStatusWidget>(PC->GetCharacterStatusUI());
			if(Status)
			{
				Status->SetHealingRange(Replenishing, GetFaith());
			}
		}
	}
}

void USNBasicAttributesComponent::GoldChanged(const FOnAttributeChangeData& Data)
{
	float Gold = Data.NewValue;
	
	if(ASNHero* HeroOwner = Cast<ASNHero>(GetOwner()))
	{
		ASNHeroController* PC = Cast<ASNHeroController>(HeroOwner->GetController());
		if(PC)
		{
			USNHeroHUD* HeroHUD = Cast<USNHeroHUD>(PC->GetHeroHUD());
			if(HeroHUD)
			{
				HeroHUD->SetGold(Gold);
			}

			USNInventoryWidget* Inventory = Cast<USNInventoryWidget>(PC->GetInventoryUI());
			if(Inventory)
			{
				Inventory->SetGold(Gold);
			}

			USNEquipmentWidget* Equipment = Cast<USNEquipmentWidget>(PC->GetEquipmentUI());
			if(Equipment)
			{
				Equipment->SetGold(Gold);
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
			USNCharacterStatusWidget* Status = Cast<USNCharacterStatusWidget>(PC->GetCharacterStatusUI());
			if(Status)
			{
				Status->SetLevelUpPoints(LevelUpPoints);
			}
		}
	}
}

void USNBasicAttributesComponent::VitalityChanged(const FOnAttributeChangeData& Data)
{
	float Vitality = Data.NewValue;
	
	if(ASNHero* HeroOwner = Cast<ASNHero>(GetOwner()))
	{
		ASNHeroController* PC = Cast<ASNHeroController>(HeroOwner->GetController());
		if(PC)
		{
			USNCharacterStatusWidget* Status = Cast<USNCharacterStatusWidget>(PC->GetCharacterStatusUI());
			if(Status)
			{
				Status->SetVitality(Vitality);
			}
			
			USNInventoryWidget* Inventory = Cast<USNInventoryWidget>(PC->GetInventoryUI());
			if(Inventory)
			{
				Inventory->SetVitality(Vitality);
			}

			USNEquipmentWidget* Equipment = Cast<USNEquipmentWidget>(PC->GetEquipmentUI());
			if(Equipment)
			{
				Equipment->SetVitality(Vitality);
			}
		}
	}
}

void USNBasicAttributesComponent::ArcaneChanged(const FOnAttributeChangeData& Data)
{
	float Arcane = Data.NewValue;
	
	if(ASNHero* HeroOwner = Cast<ASNHero>(GetOwner()))
	{
		ASNHeroController* PC = Cast<ASNHeroController>(HeroOwner->GetController());
		if(PC)
		{
			USNCharacterStatusWidget* Status = Cast<USNCharacterStatusWidget>(PC->GetCharacterStatusUI());
			if(Status)
			{
				Status->SetArcane(Arcane);
				Status->SetWeaponSpellDamage(HeroOwner->WeaponSpellDamage, Arcane);
			}

			USNInventoryWidget* Inventory = Cast<USNInventoryWidget>(PC->GetInventoryUI());
			if(Inventory)
			{
				Inventory->SetArcane(Arcane);
			}

			USNEquipmentWidget* Equipment = Cast<USNEquipmentWidget>(PC->GetEquipmentUI());
			if(Equipment)
			{
				Equipment->SetArcane(Arcane);
			}
		}
	}
}

void USNBasicAttributesComponent::MindChanged(const FOnAttributeChangeData& Data)
{
	float Mind = Data.NewValue;
	
	if(ASNHero* HeroOwner = Cast<ASNHero>(GetOwner()))
	{
		ASNHeroController* PC = Cast<ASNHeroController>(HeroOwner->GetController());
		if(PC)
		{
			USNCharacterStatusWidget* Status = Cast<USNCharacterStatusWidget>(PC->GetCharacterStatusUI());
			if(Status)
			{
				Status->SetMind(Mind);
			}

			USNInventoryWidget* Inventory = Cast<USNInventoryWidget>(PC->GetInventoryUI());
			if(Inventory)
			{
				Inventory->SetMind(Mind);
			}

			USNEquipmentWidget* Equipment = Cast<USNEquipmentWidget>(PC->GetEquipmentUI());
			if(Equipment)
			{
				Equipment->SetMind(Mind);
			}
		}
	}
}



