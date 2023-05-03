// Fill out your copyright notice in the Description page of Project Settings.


#include "Input/SNInputConfig.h"

const UInputAction* USNInputConfig::FindNativeInputActionForTag(const FGameplayTag& InputTag) const
{
	for (const FSNInputAction& Action : NativeInputActions)
	{
		if (Action.InputAction && (Action.InputTag == InputTag))
		{
			return Action.InputAction;
		}
	}
	UE_LOG(LogTemp, Error, TEXT("Can't find NativeInputAction for InputTag [%s] on InputConfig [%s]."), *InputTag.ToString(), *GetNameSafe(this));
	
	return nullptr;
}

const UInputAction* USNInputConfig::FindAbilityInputActionForTag(const FGameplayTag& InputTag) const
{
	for (const FSNInputAction& Action : AbilityInputActions)
	{
		if (Action.InputAction && (Action.InputTag == InputTag))
		{
			return Action.InputAction;
		}
	}
	UE_LOG(LogTemp, Error, TEXT("Can't find NativeInputAction for InputTag [%s] on InputConfig [%s]."), *InputTag.ToString(), *GetNameSafe(this));
	
	return nullptr;
}
