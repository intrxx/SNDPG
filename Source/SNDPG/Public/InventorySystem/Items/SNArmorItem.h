// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventorySystem/SNItemBase.h"
#include "SNArmorItem.generated.h"

/**
 * 
 */
UCLASS()
class SNDPG_API USNArmorItem : public USNItemBase
{
	GENERATED_BODY()

public:
	USNArmorItem();

protected:
	virtual void Use(ASNHero* Hero) override;
};
