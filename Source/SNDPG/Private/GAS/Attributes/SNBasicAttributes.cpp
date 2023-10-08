// Copyright 2023 Michal Oginski.


#include "GAS/Attributes/SNBasicAttributes.h"
#include "GameplayEffectExtension.h"
#include "GameplayEffect.h"
#include "Characters/SNCharacterBase.h"
#include "ActorComponents/SNBasicAttributesComponent.h"
#include "Characters/Hero/Miscellaneous/SNHeroController.h"
#include "GAS/SNAbilitySystemComponent.h"


USNBasicAttributes::USNBasicAttributes()
	: Health(200.0f)
	, MaxHealth(200.0f)
	, Resource(100.f)
	, MaxResource(100.f)
	, Stamina(150.f)
	, MaxStamina(150.f)
	, CharacterLevel(1.f)
{
	bOutOfHealth = false;
}

bool USNBasicAttributes::PreGameplayEffectExecute(FGameplayEffectModCallbackData& Data)
{
	return Super::PreGameplayEffectExecute(Data);
}

void USNBasicAttributes::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	FGameplayEffectContextHandle Context = Data.EffectSpec.GetContext();
	UAbilitySystemComponent* Source = Context.GetOriginalInstigatorAbilitySystemComponent();
	const FGameplayTagContainer& SourceTags = *Data.EffectSpec.CapturedSourceTags.GetAggregatedTags();
	FGameplayTagContainer SpecAssetTags;
	Data.EffectSpec.GetAllAssetTags(SpecAssetTags);

	// Get the Target actor, which should be our owner
	AActor* TargetActor = nullptr;
	AController* TargetController = nullptr;
	ASNCharacterBase* TargetCharacter = nullptr;
	if (Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.IsValid())
	{
		TargetActor = Data.Target.AbilityActorInfo->AvatarActor.Get();
		TargetController = Data.Target.AbilityActorInfo->PlayerController.Get();
		TargetCharacter = Cast<ASNCharacterBase>(TargetActor);
	}

	// Get the Source actor
	AActor* SourceActor = nullptr;
	AController* SourceController = nullptr;
	ASNCharacterBase* SourceCharacter = nullptr;
	if (Source && Source->AbilityActorInfo.IsValid() && Source->AbilityActorInfo->AvatarActor.IsValid())
	{
		SourceActor = Source->AbilityActorInfo->AvatarActor.Get();
		SourceController = Source->AbilityActorInfo->PlayerController.Get();
		if (SourceController == nullptr && SourceActor != nullptr)
		{
			if (APawn* Pawn = Cast<APawn>(SourceActor))
			{
				SourceController = Pawn->GetController();
			}
		}
	
	
		// Use the controller to find the source pawn
		if (SourceController)
		{
			SourceCharacter = Cast<ASNCharacterBase>(SourceController->GetPawn());
		}
		else
		{
			SourceCharacter = Cast<ASNCharacterBase>(SourceActor);
		}

		// Set the causer actor based on context if it's set
		if (Context.GetEffectCauser())
		{
			SourceActor = Context.GetEffectCauser();
		}
	}

	if (Data.EvaluatedData.Attribute == GetDamageAttribute())
	{
		// Try to extract a hit result
		FHitResult HitResult;
		if (Context.GetHitResult())
		{
			HitResult = *Context.GetHitResult();
		}

		// Store a local copy of the amount of damage done and clear the damage attribute
		const float LocalDamageDone = GetDamage();
		SetDamage(0.f);
	
		if (LocalDamageDone > 0.0f)
		{
			// If character was alive before damage is added, handle damage
			// This prevents damage being added to dead things and replaying death animations
			bool WasAlive = true;
			const USNBasicAttributesComponent* TargetAttributesComponent = USNBasicAttributesComponent::FindAttributeComponent(TargetCharacter);
			
			if (TargetCharacter)
			{
				WasAlive = TargetAttributesComponent->IsDeadOrDying();
			}

			if (TargetAttributesComponent->IsDeadOrDying())
			{
				UE_LOG(LogTemp, Warning, TEXT("%s is NOT alive when receiving damage"), *TargetCharacter->GetName());
			}

			// Apply the health change and then clamp it
			const float NewHealth = GetHealth() - LocalDamageDone;
			SetHealth(FMath::Clamp(NewHealth, 0.0f, GetMaxHealth()));
			
				// Show damage number for the Source player unless it was self damage
				if (SourceActor != TargetActor)
				{
					ASNHeroController* PC = Cast<ASNHeroController>(SourceController);
					if (PC)
					{
						//UE_LOG(LogTemp, Warning, TEXT("%s Showing floating number"), *TargetCharacter->GetName());
						PC->ShowFloatingNumber(LocalDamageDone, TargetCharacter);
					}
				}

				if ((GetHealth() <= 0.0f) && !bOutOfHealth)
				{
					if(OnOutOfHealth.IsBound())
					{
						const FGameplayEffectContextHandle& EffectContextHandle = Data.EffectSpec.GetEffectContext();
						AActor* Instigator = EffectContextHandle.GetOriginalInstigator();
						AActor* Causer = EffectContextHandle.GetEffectCauser();

						OnOutOfHealth.Broadcast(Instigator, Causer, Data.EffectSpec, Data.EvaluatedData.Magnitude);
					}
					// TargetCharacter was alive before this damage and now is not alive, give XP and Gold bounties to Source.
					// Don't give bounty to self.
					
					if (SourceController != TargetController)
					{
						// Create a dynamic instant Gameplay Effect to give the bounties
						UGameplayEffect* GEBounty = NewObject<UGameplayEffect>(GetTransientPackage(), FName(TEXT("KillBounty")));
						GEBounty->DurationPolicy = EGameplayEffectDurationType::Instant;

						int32 Index = GEBounty->Modifiers.Num();
						GEBounty->Modifiers.SetNum(Index + 2);
						
						FGameplayModifierInfo& InfoXP = GEBounty->Modifiers[Index];
						InfoXP.ModifierMagnitude = FScalableFloat(GetExperienceBounty());
						InfoXP.ModifierOp = EGameplayModOp::Additive;
						InfoXP.Attribute = USNBasicAttributes::GetExperienceAttribute();

						FGameplayModifierInfo& InfoGold = GEBounty->Modifiers[Index + 1];
						InfoGold.ModifierMagnitude = FScalableFloat(FMath::RoundToFloat(FMath::FRandRange(GetGoldBounty() - 10, GetGoldBounty() + 10)));
						InfoGold.ModifierOp = EGameplayModOp::Additive;
						InfoGold.Attribute = USNBasicAttributes::GetGoldAttribute();
						
						Source->ApplyGameplayEffectToSelf(GEBounty, 1.0f, Source->MakeEffectContext());
					}
			}
		}

		bOutOfHealth = (GetHealth() <= 0.0f);
	}// Damage
	else if(Data.EvaluatedData.Attribute == GetExperienceAttribute())
	{
		if(GetExperience() > GetMaxExperience())
		{
			UE_LOG(LogTemp, Warning, TEXT("LEVEL UP %s"), *GetOwningActor()->GetName());
			
			float XPOverMax = GetExperience() - GetMaxExperience();
			SetExperience(XPOverMax);
			
			float NewMaxXP = GetMaxExperience() * 2.0f;
			SetMaxExperience(NewMaxXP);

			float NewLevel = GetCharacterLevel()+1;
			SetCharacterLevel(NewLevel);

			// Level up reward, giving a level up point and some health
			UGameplayEffect* LevelUpReward = NewObject<UGameplayEffect>(GetTransientPackage(), FName(TEXT("LevelUpReward")));
			LevelUpReward->DurationPolicy = EGameplayEffectDurationType::Instant;
						
			int32 Idx = LevelUpReward->Modifiers.Num();
			LevelUpReward->Modifiers.SetNum(Idx + 2);
			
			FGameplayModifierInfo& InfoPoints = LevelUpReward->Modifiers[Idx];
			InfoPoints.ModifierMagnitude = FScalableFloat(1);
			InfoPoints.ModifierOp = EGameplayModOp::Additive;
			InfoPoints.Attribute = USNBasicAttributes::GetLevelUpPointsAttribute();

			FGameplayModifierInfo& InfoHealth = LevelUpReward->Modifiers[Idx + 1];
			InfoHealth.ModifierMagnitude = FScalableFloat(FMath::RoundToFloat(GetMaxHealth() / 20));
			InfoHealth.ModifierOp = EGameplayModOp::Additive;
			InfoHealth.Attribute = USNBasicAttributes::GetMaxHealthAttribute();
						
			Source->ApplyGameplayEffectToSelf(LevelUpReward, 1.0f, Source->MakeEffectContext());

			// Heal Character after lvling up
			SetHealth(GetMaxHealth());
		}
	}
	else if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		// Handle other health changes.
		// Health loss should go through Damage.
		SetHealth(FMath::Clamp(GetHealth(), 0.0f, GetMaxHealth()));
	} // Health
	else if (Data.EvaluatedData.Attribute == GetResourceAttribute())
	{
		// Handle mana changes.
		SetResource(FMath::Clamp(GetResource(), 0.0f, GetMaxResource()));
	} // Mana
	else if(Data.EvaluatedData.Attribute == GetStaminaAttribute())
	{
		SetStamina(FMath::Clamp(GetStamina(), 0.0f, GetMaxStamina()));
	}
}

void USNBasicAttributes::PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const
{
	Super::PreAttributeBaseChange(Attribute, NewValue);

	ClampAttribute(Attribute, NewValue);
}

void USNBasicAttributes::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);
	
	ClampAttribute(Attribute, NewValue);
}

void USNBasicAttributes::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	Super::PostAttributeChange(Attribute, OldValue, NewValue);

	if(Attribute == GetMaxHealthAttribute())
	{
		USNAbilitySystemComponent* SNASC = GetSNAbilitySystemComponent();
		check(SNASC);

		SNASC->ApplyModToAttribute(GetHealthAttribute(), EGameplayModOp::Override, NewValue);
	}
	else if(Attribute == GetMaxResourceAttribute())
	{
		USNAbilitySystemComponent* SNASC = GetSNAbilitySystemComponent();
		check(SNASC);

		SNASC->ApplyModToAttribute(GetResourceAttribute(), EGameplayModOp::Override, NewValue);
	}
	else if(Attribute == GetMaxStaminaAttribute())
	{
		USNAbilitySystemComponent* SNASC = GetSNAbilitySystemComponent();
		check(SNASC);

		SNASC->ApplyModToAttribute(GetStaminaAttribute(), EGameplayModOp::Override, NewValue);
	}

	if(bOutOfHealth && (GetHealth() > 0.0f))
	{
		bOutOfHealth = false;
	}
}

void USNBasicAttributes::ClampAttribute(const FGameplayAttribute& Attribute, float& NewValue) const
{
	if(Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxHealth());
	}
	else if(Attribute == GetMaxHealthAttribute())
	{
		NewValue = FMath::Max(NewValue, 1.0f);
	}
	else if(Attribute == GetResourceAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxResource());
	}
	else if(Attribute == GetMaxResourceAttribute())
	{
		NewValue = FMath::Max(NewValue, 1.0f);
	}
	else if(Attribute == GetStaminaAttribute())
	{
		NewValue = FMath::Max(NewValue, 1.0f);
	}
	else if(Attribute == GetMaxStaminaAttribute())
	{
		NewValue = FMath::Max(NewValue, 1.0f);
	}
}
