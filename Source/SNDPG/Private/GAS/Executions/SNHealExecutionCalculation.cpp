// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Executions/SNHealExecutionCalculation.h"
#include "GAS/SNAbilitySystemComponent.h"
#include "GAS/Attributes/SNBasicAttributes.h"

struct SNHealStatics
{
	FGameplayEffectAttributeCaptureDefinition HealingDef;
	FGameplayEffectAttributeCaptureDefinition FaithDef;
	
	SNHealStatics()
	{
		HealingDef = FGameplayEffectAttributeCaptureDefinition(USNBasicAttributes::GetHealingAttribute(), EGameplayEffectAttributeCaptureSource::Source, true);
		FaithDef = FGameplayEffectAttributeCaptureDefinition(USNBasicAttributes::GetFaithAttribute(), EGameplayEffectAttributeCaptureSource::Source, true);
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
	RelevantAttributesToCapture.Add(HealingStatics().FaithDef);
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

	float Faith = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(HealingStatics().FaithDef, EvaluateParameters, Faith);
	Faith = FMath::Max<float>(Faith, 0.0f);
	
	float HealingDone = Healing + FMath::FRandRange(0, Faith);

	HealingDone = FMath::RoundToFloat(HealingDone);
	
	if(HealingDone > 0.0f)
	{
		OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(USNBasicAttributes::GetHealthAttribute(), EGameplayModOp::Additive, HealingDone));
		
		USNAbilitySystemComponent* SourceASC = Cast<USNAbilitySystemComponent>(SourceAbilitySystemComponent);
		if(SourceASC)
		{
			SourceASC->ReceivedHeal(SourceASC, HealingDone);
		}
	}
}
