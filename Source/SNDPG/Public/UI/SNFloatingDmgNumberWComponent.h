// Copyright 2023 Michal Oginski.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "SNFloatingDmgNumberWComponent.generated.h"

/**
 * 
 */
UCLASS()
class SNDPG_API USNFloatingDmgNumberWComponent : public UWidgetComponent
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetDamageText(float Damage);
	
};
