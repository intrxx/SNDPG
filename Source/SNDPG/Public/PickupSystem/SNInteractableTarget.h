// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Abilities/GameplayAbility.h"
#include "SNInteractionOption.h"
#include "SNInteractableTarget.generated.h"

struct FSNInteractionQuery;

class FSNInteractionOptionBuilder
{
public:
	FSNInteractionOptionBuilder(TScriptInterface<ISNInteractableTarget> InterfaceTargetScope, TArray<FSNInteractionOption>& InteractOptions)
		: Scope(InterfaceTargetScope)
		, Options(InteractOptions)
	{
	}

	void AddInteractionOption(const FSNInteractionOption& Option)
	{
		FSNInteractionOption& OptionEntry = Options.Add_GetRef(Option);
		OptionEntry.InteractableTarget = Scope;
	}

private:
	TScriptInterface<ISNInteractableTarget> Scope;
	TArray<FSNInteractionOption>& Options;
};

// This class does not need to be modified.
UINTERFACE(MinimalAPI, meta = (CannotImplementInterfaceInBlueprint))
class USNInteractableTarget : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SNDPG_API ISNInteractableTarget
{
	GENERATED_BODY()

public:
	/**  */
	virtual void GatherInteractionOptions(const FSNInteractionQuery& InteractQuery, FSNInteractionOptionBuilder& OptionBuilder) = 0;

	/**  */
	virtual void CustomizeInteractionEventData(const FGameplayTag& InteractionEventTag, FGameplayEventData& InOutEventData) { }
};
