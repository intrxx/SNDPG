// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SNHeroHUD.generated.h"

/**
 * 
 */
UCLASS()
class SNDPG_API USNHeroHUD : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void ToggleCharacterStatusEvent(bool bIsVisible);
	
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetHealth(float Health);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetMaxHealth(float MaxHealth);
	
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetHealthPercentage(float HealthPercentage);
	
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetResource(float Resource);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetMaxResource(float MaxResource);
	
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetResourcePercentage(float ResourcePercentage);

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
	void SetHealingRange(float Healing, float Faith);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetLevelUpPoints(float LevelUpPoints);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetVitality(float Vitality);

public:
	bool bIsCharacterStatusVisible = false;
};
