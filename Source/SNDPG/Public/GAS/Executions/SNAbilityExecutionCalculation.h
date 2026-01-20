// Copyright 2026 out of sCope team - intrxx

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "SNAbilityExecutionCalculation.generated.h"

/**
 * 
 */
UCLASS()
class SNDPG_API USNAbilityExecutionCalculation : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()

	USNAbilityExecutionCalculation();

public:
	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
};
