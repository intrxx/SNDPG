// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Executions/SNDamageExecutionCalculation.h"

#include "GAS/SNAbilitySystemComponent.h"
#include "GAS/Attributes/SNBasicAttributes.h"


struct SNDamageStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(Damage)
	DECLARE_ATTRIBUTE_CAPTUREDEF(Armor)

	SNDamageStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(USNBasicAttributes, Damage, Source, true)
		DEFINE_ATTRIBUTE_CAPTUREDEF(USNBasicAttributes, Armor, Target, true)
	}
};

static const SNDamageStatics& DamageStatics()
{
	static SNDamageStatics SNDamageStats;
	return SNDamageStats;
}
USNDamageExecutionCalculation::USNDamageExecutionCalculation()
{
	RelevantAttributesToCapture.Add(DamageStatics().DamageDef);
	RelevantAttributesToCapture.Add(DamageStatics().ArmorDef);
}

void USNDamageExecutionCalculation::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
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

	float Armor = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().ArmorDef, EvaluateParameters, Armor);
	Armor = FMath::Max<float>(Armor, 0.0f);

	float Damage = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().DamageDef, EvaluateParameters, Damage);
	Damage = FMath::Max<float>(Damage, 0.0f);

	// Apply any buffs here
	float UnmitigatedDamage = Damage;

	//TODO Need to think of some formula to mitigate damage by armor
	float MitigatedDamage = UnmitigatedDamage;

	if(MitigatedDamage > 0.0f)
	{
		OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(DamageStatics().DamageProperty, EGameplayModOp::Additive, MitigatedDamage));

		USNAbilitySystemComponent* TargetASC = Cast<USNAbilitySystemComponent>(TargetAbilitySystemComponent);
		check(TargetASC);

		USNAbilitySystemComponent* SourceASC = Cast<USNAbilitySystemComponent>(SourceAbilitySystemComponent);
		check(SourceASC);

		TargetASC->ReceivedDamage(SourceASC, UnmitigatedDamage, MitigatedDamage);
	}
}
