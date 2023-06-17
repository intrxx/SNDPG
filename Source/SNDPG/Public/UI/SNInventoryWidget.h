// Copyright 2023 Michał Ogiński.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SNInventoryWidget.generated.h"

/**
 * 
 */
UCLASS()
class SNDPG_API USNInventoryWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void ToggleCharacterInventory(bool bIsVisible);
	
	// Updates the UI for currently selected item
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void UpdateCurrentItemInfo(USNItemBase* ItemBase);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetGold(float Gold);
	
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetCharacterLevel(float CharacterLevel);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetExperience(float SoulsPossessed);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetVitality(float Vitality);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetMind(float Mind);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetEndurance(float Endurance);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetStrength(float Strength);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetFaith(float Faith);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetArcane(float Arcane);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetHealth(float CurrentHealth, float MaxHealth);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetResource(float CurrentMana, float MaxMana);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetStamina(float CurrentStamina, float MaxStamina);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetArmour(float Armour);
	
public:
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	bool bIsCharacterInventoryVisible = false;
};
