// Copyright 2023 Michał Ogiński.

#pragma once

#include "CoreMinimal.h"
#include "InventorySystem/SNItemBase.h"
#include "SNWeaponItem.generated.h"

/**
 * 
 */
UCLASS()
class SNDPG_API USNWeaponItem : public USNItemBase
{
	GENERATED_BODY()

public:
	USNWeaponItem();

public:
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "SN|Item|Weapon")
	float LightAttackDamage = 0.0f;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "SN|Item|Weapon")
	float HeavyAttackDamage = 0.0f;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "SN|Item|Weapon")
	float WeaponSpellDamage = 0.0f;
	
protected:
	virtual void Use(ASNHero* Hero) override;
	
};
