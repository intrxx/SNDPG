// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "AbilitySystemInterface.h"
#include "SNHeroState.generated.h"

/**
 * 
 */
UCLASS()
class SNDPG_API ASNHeroState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	ASNHeroState();

	class UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	//class USNArenaAttributeSet* GetAttributeSet() const;

	/**
	* Getters for attributes from GAttributeSet.
	*/

	//UFUNCTION(BlueprintCallable, Category = "PlayerState|Attributes")
	//float GetHealth() const;

protected:
	UPROPERTY()
	class USNAbilitySystemComponent* AbilitySystemComponent;

	//UPROPERTY()
	//class USNArenaAttributeSet* AttributeSet;
};
