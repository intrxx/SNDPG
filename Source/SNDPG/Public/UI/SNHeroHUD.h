// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SNHeroHUD.generated.h"

/**
 * 
 */
UCLASS()
class SNDPG_API USNHeroHUD : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetHealth(float Health);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetMaxHealth(float MaxHealth);
	
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetHealthPercentage(float HealthPercentage);


	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetResource(float Resource);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetMaxResource(float MaxResource);
	
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetResourcePercentage(float ResourcePercentage);
	
};
