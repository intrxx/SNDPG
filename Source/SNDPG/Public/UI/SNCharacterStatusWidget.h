// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SNCharacterStatusWidget.generated.h"

/**
 * 
 */
UCLASS()
class SNDPG_API USNCharacterStatusWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void ToggleCharacterStatus(bool bIsVisible);
	
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetHealth(float Health);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetMaxHealth(float MaxHealth);
	
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetResource(float Resource);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetMaxResource(float MaxResource);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetStamina(float Stamina);
	
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetMaxStamina(float MaxStamina);
	
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetExperience(float Experience);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetMaxExperience(float MaxExperience);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetStrength(float Strength);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetFaith(float Faith);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetCharacterLevel(float CharacterLevel);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetEndurance(float Endurance);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetArmour(float Armour);
	
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetLevelUpPoints(float LevelUpPoints);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetVitality(float Vitality);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetMind(float Mind);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetArcane(float Arcane);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetHealingRange(float Healing, float Faith);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetReplenishingRange(float Replenishing, float Faith);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetR1Range(float R1Damage, float Strength);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetR2Range(float R2Damage, float Strength);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetL1Range(float L1Damage, float Strength);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetWeaponSpellDamage(float WeaponSpellDamage, float Arcane);

public:
	bool bIsCharacterStatusVisible = false;
};
