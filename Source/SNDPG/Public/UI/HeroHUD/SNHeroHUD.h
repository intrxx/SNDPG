// Copyright 2023 Michał Ogiński.

#pragma once

#include "CoreMinimal.h"
#include "UI/SNStatusWidget.h"
#include "SNHeroHUD.generated.h"

class USNItemBase;
/**
 * 
 */
UCLASS()
class SNDPG_API USNHeroHUD : public USNStatusWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "SN|UI|CharacterStats")
	void SetEquippedItemDisplay(USNItemBase* Item);
	
};
