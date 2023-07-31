// Copyright 2023 Michal Oginski.

#pragma once

#include "CoreMinimal.h"
#include "GAS/Attributes/SNMainAttributeSet.h"
#include "SNBasicAttributes.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class SNDPG_API USNBasicAttributes : public USNMainAttributeSet
{
	GENERATED_BODY()

public:
	USNBasicAttributes();
	
	ATTRIBUTE_ACCESSORS(USNBasicAttributes, Health);
	ATTRIBUTE_ACCESSORS(USNBasicAttributes, MaxHealth);
	ATTRIBUTE_ACCESSORS(USNBasicAttributes, Resource);
	ATTRIBUTE_ACCESSORS(USNBasicAttributes, MaxResource);
	ATTRIBUTE_ACCESSORS(USNBasicAttributes, Healing);
	ATTRIBUTE_ACCESSORS(USNBasicAttributes, Damage);
	ATTRIBUTE_ACCESSORS(USNBasicAttributes, Armour);
	ATTRIBUTE_ACCESSORS(USNBasicAttributes, Experience);
	ATTRIBUTE_ACCESSORS(USNBasicAttributes, MaxExperience);
	ATTRIBUTE_ACCESSORS(USNBasicAttributes, Faith);
	ATTRIBUTE_ACCESSORS(USNBasicAttributes, Endurance);
	ATTRIBUTE_ACCESSORS(USNBasicAttributes, Strength);
	ATTRIBUTE_ACCESSORS(USNBasicAttributes, CharacterLevel);
	ATTRIBUTE_ACCESSORS(USNBasicAttributes, ExperienceBounty);
	ATTRIBUTE_ACCESSORS(USNBasicAttributes, LevelUpPoints);
	ATTRIBUTE_ACCESSORS(USNBasicAttributes, Vitality);
	ATTRIBUTE_ACCESSORS(USNBasicAttributes, Stamina);
	ATTRIBUTE_ACCESSORS(USNBasicAttributes, MaxStamina);
	ATTRIBUTE_ACCESSORS(USNBasicAttributes, Mind);
	ATTRIBUTE_ACCESSORS(USNBasicAttributes, Arcane);
	ATTRIBUTE_ACCESSORS(USNBasicAttributes, Replenishing);
	ATTRIBUTE_ACCESSORS(USNBasicAttributes, Gold);
	ATTRIBUTE_ACCESSORS(USNBasicAttributes, GoldBounty);

	// Delegate to broadcast when the health attribute reaches zero.
	mutable FSNAttributeEvent OnOutOfHealth;
	
protected:
	void AdjustAttributeForMaxChange(FGameplayAttributeData& AffectedAttribute, const FGameplayAttributeData& MaxAttribute, float NewMaxValue, const FGameplayAttribute& AffectedAttributeProperty);
	
	virtual bool PreGameplayEffectExecute(FGameplayEffectModCallbackData& Data) override;
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

	virtual void PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const override;
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;

	void ClampAttribute(const FGameplayAttribute& Attribute, float& NewValue) const;

private:
	bool bOutOfHealth = false;
	
public:
	// The current health attribute.  The health will be capped by the max health attribute.  Health is hidden from modifiers so only executions can modify it.
	UPROPERTY(BlueprintReadOnly, Category = "SN|BasicAttributes", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Health;

	// The current max health attribute.  Max health is an attribute since gameplay effects can modify it.
	UPROPERTY(BlueprintReadOnly,  Category = "SN|BasicAttributes", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxHealth;

	UPROPERTY(BlueprintReadOnly,  Category = "SN|BasicAttributes", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Resource;

	UPROPERTY(BlueprintReadOnly,  Category = "SN|BasicAttributes", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxResource;

	UPROPERTY(BlueprintReadOnly,  Category = "SN|BasicAttributes", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Stamina;

	UPROPERTY(BlueprintReadOnly,  Category = "SN|BasicAttributes", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxStamina;

	UPROPERTY(BlueprintReadOnly,  Category = "SN|BasicAttributes", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Experience;

	UPROPERTY(BlueprintReadOnly,  Category = "SN|BasicAttributes", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxExperience;

	UPROPERTY(BlueprintReadOnly, Category = "SN|BasicAttributes", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData CharacterLevel;
	
	UPROPERTY(BlueprintReadOnly, Category = "SN|BasicAttributes", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Strength;

	UPROPERTY(BlueprintReadOnly, Category = "SN|BasicAttributes", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Vitality;
	
	UPROPERTY(BlueprintReadOnly, Category = "SN|BasicAttributes", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Endurance;
	
	UPROPERTY(BlueprintReadOnly, Category = "SN|BasicAttributes", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Faith;
	
	UPROPERTY(BlueprintReadOnly, Category = "SN|BasicAttributes", meta=(AllowPrivateAccess=true))
	FGameplayAttributeData Armour;
	
	UPROPERTY(BlueprintReadOnly, Category = "SN|BasicAttributes", meta=(AllowPrivateAccess=true))
	FGameplayAttributeData Healing;

	UPROPERTY(BlueprintReadOnly, Category = "SN|BasicAttributes", meta=(AllowPrivateAccess=true))
	FGameplayAttributeData Replenishing;
	
	UPROPERTY(BlueprintReadOnly, Category = "SN|BasicAttributes", meta=(AllowPrivateAccess=true))
	FGameplayAttributeData Damage;

	UPROPERTY(BlueprintReadOnly, Category = "SN|BasicAttributes", meta=(AllowPrivateAccess=true))
	FGameplayAttributeData ExperienceBounty;

	UPROPERTY(BlueprintReadOnly, Category = "SN|BasicAttributes", meta=(AllowPrivateAccess=true))
	FGameplayAttributeData LevelUpPoints;

	UPROPERTY(BlueprintReadOnly, Category = "SN|BasicAttributes", meta=(AllowPrivateAccess=true))
	FGameplayAttributeData Arcane;

	UPROPERTY(BlueprintReadOnly, Category = "SN|BasicAttributes", meta=(AllowPrivateAccess=true))
	FGameplayAttributeData Mind;

	UPROPERTY(BlueprintReadOnly, Category = "SN|BasicAttributes", meta=(AllowPrivateAccess=true))
	FGameplayAttributeData Gold;

	UPROPERTY(BlueprintReadOnly, Category = "SN|BasicAttributes", meta=(AllowPrivateAccess=true))
	FGameplayAttributeData GoldBounty;
};
