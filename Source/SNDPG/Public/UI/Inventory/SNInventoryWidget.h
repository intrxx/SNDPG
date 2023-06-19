// Copyright 2023 Michał Ogiński.

#pragma once

#include "CoreMinimal.h"
#include "UI/SNStatusWidget.h"
#include "SNInventoryWidget.generated.h"

/**
 * 
 */
UCLASS()
class SNDPG_API USNInventoryWidget : public USNStatusWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "SN|UI|Inventory|Visibility")
	void ToggleCharacterInventory(bool bIsVisible);
	
	// Updates the UI for currently selected item
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "SN|UI|Inventory|")
	void UpdateCurrentItemInfo(USNItemBase* ItemBase);

public:
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "SN|UI|Inventory|Visibility")
	bool bIsCharacterInventoryVisible = false;
};
