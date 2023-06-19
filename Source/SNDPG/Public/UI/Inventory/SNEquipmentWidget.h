// Copyright 2023 Michał Ogiński.

#pragma once

#include "CoreMinimal.h"
#include "UI/SNStatusWidget.h"
#include "SNEquipmentWidget.generated.h"

/**
 * 
 */
UCLASS()
class SNDPG_API USNEquipmentWidget : public USNStatusWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "SN|UI|Equipment|Visibility")
	void ToggleEquipment(bool bIsVisible);

public:
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "SN|UI|Equipment|Visibility")
	bool bIsEquipmentVisible = false;
};
