// Copyright 2023 Michal Oginski.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SNHealthBarWidget.generated.h"

/**
 * 
 */
UCLASS()
class SNDPG_API USNHealthBarWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetHealthPercentage(float HealthPercentage);
};
