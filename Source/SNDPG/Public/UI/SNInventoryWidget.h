// Copyright 2023 Michał Ogiński.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SNInventoryWidget.generated.h"

/**
 * 
 */
UCLASS()
class SNDPG_API USNInventoryWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void ToggleCharacterInventory(bool bIsVisible);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetGold(float Gold);

public:
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	bool bIsCharacterInventoryVisible = false;
};
