// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventorySystem/SNItemBase.h"
#include "SNTalismanItem.generated.h"

/**
 * 
 */
UCLASS()
class SNDPG_API USNTalismanItem : public USNItemBase
{
	GENERATED_BODY()

public:
	USNTalismanItem();

protected:
	virtual void Use(ASNHero* Hero) override;
};
