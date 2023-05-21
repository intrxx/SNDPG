// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Executions/SNDamageExecutionCalculation.h"

#include "GAS/SNAbilitySystemComponent.h"
#include "GAS/Attributes/SNBasicAttributes.h"


struct SNDamageStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(Damage);
	
	FGameplayEffectAttributeCaptureDefinition ArmourDef;
	FGameplayEffectAttributeCaptureDefinition StrengthDef;
	FGameplayEffectAttributeCaptureDefinition EnduranceDef;

	SNDamageStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(USNBasicAttributes, Damage, Source, true);

		ArmourDef = FGameplayEffectAttributeCaptureDefinition(USNBasicAttributes::GetArmourAttribute(), EGameplayEffectAttributeCaptureSource::Target, false);
		StrengthDef = FGameplayEffectAttributeCaptureDefinition(USNBasicAttributes::GetStrengthAttribute(), EGameplayEffectAttributeCaptureSource::Source, true);
		EnduranceDef = FGameplayEffectAttributeCaptureDefinition(USNBasicAttributes::GetEnduranceAttribute(), EGameplayEffectAttributeCaptureSource::Target, false);
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
	RelevantAttributesToCapture.Add(DamageStatics().ArmourDef);
	RelevantAttributesToCapture.Add(DamageStatics().StrengthDef);
	RelevantAttributesToCapture.Add(DamageStatics().EnduranceDef);
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

	float Armour = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().ArmourDef, EvaluateParameters, Armour);
	Armour = FMath::Max<float>(Armour, 0.0f);

	UE_LOG(LogTemp, Warning, TEXT("Armoud on target: %f"), Armour);

	float Damage = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().DamageDef, EvaluateParameters, Damage);
	Damage += FMath::Max<float>(Spec.GetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Ability.Data.Damage")), true, -1.0f), 0.0f);
	
	float StrengthBonus = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().StrengthDef, EvaluateParameters, StrengthBonus);
	StrengthBonus = FMath::Max<float>(StrengthBonus, 0.0f);

	float Endurance = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().EnduranceDef, EvaluateParameters, Endurance);
	Endurance = FMath::Max<float>(Endurance, 0.0f);
	
	// Apply buffs and debuffs below
	float UnmitigatedDamage = Damage + StrengthBonus;
	
	float MitigatedDamage = UnmitigatedDamage - Endurance;
	if(Armour > 0)
	{
		MitigatedDamage -= Armour/100;
	}

	MitigatedDamage = FMath::RoundToFloat(MitigatedDamage);
	
	if(MitigatedDamage > 0.0f)
	{
		OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(DamageStatics().DamageProperty, EGameplayModOp::Additive, MitigatedDamage));

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
