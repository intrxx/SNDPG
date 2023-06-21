// Fill out your copyright notice in the Description page of Project Settings.

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

protected:
	virtual void Use(ASNHero* Hero) override;
};
