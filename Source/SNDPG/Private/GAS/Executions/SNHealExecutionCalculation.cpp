// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Executions/SNHealExecutionCalculation.h"
#include "GAS/SNAbilitySystemComponent.h"
#include "GAS/Attributes/SNBasicAttributes.h"

struct SNHealStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(Healing);

	SNHealStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(USNBasicAttributes, Healing, Source, false);
	}
};

static const SNHealStatics& HealingStatics()
{
	static SNHealStatics SnHealStatics;
	return SnHealStatics;
}

USNHealExecutionCalculation::USNHealExecutionCalculation()
{
	RelevantAttributesToCapture.Add(HealingStatics().HealingDef);
}

void USNHealExecutionCalculation::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	Super::Execute_Implementation(ExecutionParams, OutExecutionOutput);

	UAbilitySystemComponent* TargetAbilitySystemComponent = ExecutionParams.GetTargetAbilitySystemComponent();
	UAbilitySystemComponent* SourceAbilitySystemComponent = ExecutionParams.GetSourceAbilitySystemComponent();

	AActor* TargetActor = TargetAbilitySystemComponent ? TargetAbilitySystemComponent->GetAvatarActor() : nullptr;
	AActor* SourceActor = SourceAbilitySystemComponent ? SourceAbilitySystemComponent->GetAvatarActor() : nullptr;

	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();

	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluateParameters;
	EvaluateParameters.TargetTags = TargetTags;
	EvaluateParameters.SourceTags = SourceTags;

	float Healing = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(HealingStatics().HealingDef, EvaluateParameters, Healing);
	Healing = FMath::Max<float>(Healing, 0.0f);

	float UnmitigatedHealing = Healing;

	float MitigatedHealing = UnmitigatedHealing;
	
	if(MitigatedHealing > 0.0f)
	{
		OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(HealingStatics().HealingProperty, EGameplayModOp::Additive, MitigatedHealing));

		USNAbilitySystemComponent* TargetASC = Cast<USNAbilitySystemComponent>(TargetAbilitySystemComponent);
		check(TargetASC);

		USNAbilitySystemComponent* SourceASC = Cast<USNAbilitySystemComponent>(SourceAbilitySystemComponent);
		check(SourceASC);

		TargetASC->ReceivedHeal(SourceASC, UnmitigatedHealing, MitigatedHealing);
	}
}
