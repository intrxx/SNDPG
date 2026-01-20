// Copyright 2026 out of sCope team - intrxx

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SNInGameMenu.generated.h"

/**
 * 
 */
UCLASS()
class SNDPG_API USNInGameMenu : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void ToggleInGameMenu(bool bIsVisible);

public:
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	bool bIsInGameMenuVisible = false;
};
