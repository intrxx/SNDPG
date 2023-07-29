// Copyright 2023 Michał Ogiński.

#pragma once

#include "CoreMinimal.h"
#include "InventorySystem/SNItemBase.h"
#include "SNThrowingKnifeItem.generated.h"

/**
 * 
 */
UCLASS()
class SNDPG_API USNThrowingKnifeItem : public USNItemBase
{
	GENERATED_BODY()

public:
	USNThrowingKnifeItem();

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "SN|Item|ThrowingDagger")
	float ThrowingWeaponDamage = 0.0f;

protected:
	virtual void Use(ASNHero* Hero) override;
	
};
