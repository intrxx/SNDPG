// Copyright 2023 Michał Ogiński.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SNStatusWidget.generated.h"

/**
 * 
 */
UCLASS()
class SNDPG_API USNStatusWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	// Percentages
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "SN|UI|Percentages")
	void SetHealthPercentage(float HealthPercentage);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "SN|UI|Percentages")
	void SetResourcePercentage(float ResourcePercentage);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "SN|UI|Percentages")
	void SetStaminaPercentage(float StaminaPercentage);

	// Main Attributes
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "SN|UI|MainAttributes")
	void SetHealth(float CurrentHealth, float MaxHealth);
	
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "SN|UI|MainAttributes")
	void SetMaxHealth(float MaxHealth);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "SN|UI|MainAttributes")
	void SetResource(float CurrentResource, float MaxResource);
	
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "SN|UI|MainAttributes")
	void SetMaxResource(float MaxResource);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "SN|UI|MainAttributes")
	void SetStamina(float Stamina);
	
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "SN|UI|MainAttributes")
	void SetMaxStamina(float MaxStamina);

	// Character Stats
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "SN|UI|CharacterStats")
	void SetCharacterLevel(float CharacterLevel);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "SN|UI|CharacterStats")
	void SetExperience(float Experience);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "SN|UI|CharacterStats")
	void SetMaxExperience(float MaxExperience);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "SN|UI|CharacterStats")
	void SetLevelUpPoints(float LevelUpPoints);

	// Combat Attributes
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "SN|UI|CombatAttributes")
	void SetVitality(float Vitality);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "SN|UI|CombatAttributes")
	void SetMind(float Mind);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "SN|UI|CombatAttributes")
	void SetEndurance(float Endurance);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "SN|UI|CombatAttributes")
	void SetStrength(float Strength);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "SN|UI|CombatAttributes")
	void SetFaith(float Faith);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "SN|UI|CombatAttributes")
	void SetArcane(float Arcane);

	// Misc
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "SN|UI|Misc")
	void SetGold(float Gold);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "SN|UI|Misc")
	void SetArmour(float Armour);
};
