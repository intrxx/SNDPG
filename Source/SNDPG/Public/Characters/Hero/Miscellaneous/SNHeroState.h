// Copyright 2023 Michal Oginski.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "AbilitySystemInterface.h"
#include "SNHeroState.generated.h"

class USNAbilitySystemComponent;
/**
 * 
 */
UCLASS()
class SNDPG_API ASNHeroState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	ASNHeroState();
	
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

protected:
	UPROPERTY()
	USNAbilitySystemComponent* AbilitySystemComponent;
	
};
