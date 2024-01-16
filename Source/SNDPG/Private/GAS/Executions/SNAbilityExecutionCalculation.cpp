// Copyright 2023 Michal Oginski.


#include "GAS/Executions/SNAbilityExecutionCalculation.h"

#include "Engine/ICookInfo.h"
#include "GAS/SNAbilitySystemComponent.h"
#include "GAS/Attributes/SNBasicAttributes.h"
#include "Kismet/GameplayStatics.h"
#include "Widgets/Text/ISlateEditableTextWidget.h"

struct SNAbilityStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(Damage);

	FGameplayEffectAttributeCaptureDefinition ArcaneDef;
	FGameplayEffectAttributeCaptureDefinition EnduranceDef;

	SNAbilityStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(USNBasicAttributes, Damage, Source, true);

		ArcaneDef = FGameplayEffectAttributeCaptureDefinition(USNBasicAttributes::GetArcaneAttribute(), EGameplayEffectAttributeCaptureSource::Source, true);
		EnduranceDef = FGameplayEffectAttributeCaptureDefinition(USNBasicAttributes::GetEnduranceAttribute(), EGameplayEffectAttributeCaptureSource::Target, false);
	}
};

static const SNAbilityStatics& AbilityStatics()
{
	static SNAbilityStatics AbilityStatics;
	return AbilityStatics;
}

USNAbilityExecutionCalculation::USNAbilityExecutionCalculation()
{
	RelevantAttributesToCapture.Add(AbilityStatics().ArcaneDef);
	RelevantAttributesToCapture.Add(AbilityStatics().DamageDef);
	RelevantAttributesToCapture.Add(AbilityStatics().EnduranceDef);
}

void USNAbilityExecutionCalculation::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
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

	float Arcane = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(AbilityStatics().ArcaneDef, EvaluateParameters, Arcane);
	Arcane = FMath::Max<float>(Arcane, 0.0f);
	
	float Damage = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(AbilityStatics().DamageDef, EvaluateParameters, Damage);
	Damage += FMath::Max<float>(Spec.GetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Ability.Data.Damage")),
		true, -1.0f), 0.0f);
	
	float Endurance = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(AbilityStatics().EnduranceDef, EvaluateParameters, Endurance);
	Endurance = FMath::Max<float>(Endurance, 0.0f);
	
	// Apply buffs and debuffs below
	float UnmitigatedDamage = Damage + FMath::FRandRange(0, Arcane);
	
	float MitigatedDamage = UnmitigatedDamage - FMath::FRandRange(0, Endurance);
	
	MitigatedDamage = FMath::RoundToFloat(MitigatedDamage);
	
	if(MitigatedDamage > 0.0f)
	{
		OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(AbilityStatics().DamageProperty, EGameplayModOp::Additive, MitigatedDamage));

		USNAbilitySystemComponent* TargetASC = Cast<USNAbilitySystemComponent>(TargetAbilitySystemComponent);
		if(TargetASC)
		{
			USNAbilitySystemComponent* SourceASC = Cast<USNAbilitySystemComponent>(SourceAbilitySystemComponent);
			if(SourceASC)
			{
				TargetASC->ReceivedDamage(SourceASC, UnmitigatedDamage, MitigatedDamage);
			}
		}
	}
}
