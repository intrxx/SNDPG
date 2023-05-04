// Copyright 2023 Michal Oginski.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "SNMainAttributeSet.generated.h"

class USNAbilitySystemComponent;
struct FGameplayEffectSpec;

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)


// Delegate used to broadcast attribute events.
DECLARE_MULTICAST_DELEGATE_FourParams(FLyraAttributeEvent, AActor* /*EffectInstigator*/, AActor* /*EffectCauser*/, const FGameplayEffectSpec& /*EffectSpec*/, float /*EffectMagnitude*/);

/**
 * USNMainAttributeSet
 *
 * Base attribute set class for the project.
 */
UCLASS()
class SNDPG_API USNMainAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:

	USNMainAttributeSet();

	UWorld* GetWorld() const override;

	USNAbilitySystemComponent* GetSNAbilitySystemComponent() const;
};
