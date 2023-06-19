// Copyright 2023 Michał Ogiński.

#pragma once

#include "CoreMinimal.h"
#include "UI/SNStatusWidget.h"
#include "SNCharacterStatusWidget.generated.h"

/**
 * 
 */
UCLASS()
class SNDPG_API USNCharacterStatusWidget : public USNStatusWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "SN|UI|Status|Visibility")
	void ToggleCharacterStatus(bool bIsVisible);
	
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "SN|UI|Status|CombatRanges")
	void SetHealingRange(float Healing, float Faith);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "SN|UI|Status|CombatRanges")
	void SetReplenishingRange(float Replenishing, float Faith);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "SN|UI|Status|CombatRanges")
	void SetR1Range(float R1Damage, float Strength);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "SN|UI|Status|CombatRanges")
	void SetR2Range(float R2Damage, float Strength);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "SN|UI|Status|CombatRanges")
	void SetL1Range(float L1Damage, float Strength);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "SN|UI|Status|CombatRanges")
	void SetWeaponSpellDamage(float WeaponSpellDamage, float Arcane);

public:
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "SN|UI|Status|Visibility")
	bool bIsCharacterStatusVisible = false;
};
