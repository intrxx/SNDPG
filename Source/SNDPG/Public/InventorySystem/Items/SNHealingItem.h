// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventorySystem/SNItemBase.h"
#include "SNHealingItem.generated.h"

class USNGameplayAbility;
/**
 * 
 */
UCLASS()
class SNDPG_API USNHealingItem : public USNItemBase
{
	GENERATED_BODY()

public:
	USNHealingItem();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SN|HealingItem", meta = (ClampMin = 0.0f))
	float HealingAmount = 0.0f;

protected:
	virtual void Use(ASNHero* Hero) override;
};
