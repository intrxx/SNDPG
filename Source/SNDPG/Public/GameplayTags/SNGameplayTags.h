// Copyright 2023 Michal Oginski.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

class UGameplayTagsManager;
/**
 * 
 */
struct SNDPG_API FSNGameplayTags
{
public:
	static const FSNGameplayTags& Get() {return GameplayTags;}
	static void InitializeNativeTags();
	static FGameplayTag FindTagByString(FString TagString, bool bMatchPartialString = false);

public:
	// Input
	FGameplayTag Input_Move;
	FGameplayTag Input_Look_Stick;
	FGameplayTag Input_Look_Mouse;
	FGameplayTag Input_ToggleCharacterStatus;

	// Events
	FGameplayTag GameplayEvent_Death;

	// Abilities helper tags
	FGameplayTag Ability_Behavior_SurvivesDeath;
	FGameplayTag Ability_Behavior_BlockStaminaRegen;
	
	// Abilities
	FGameplayTag Ability_Input_Melee;
	FGameplayTag Ability_Input_Healing;

	// Cooldowns
	FGameplayTag Ability_CD_Healing;

	// Data
	FGameplayTag HitTag_HeroMelee;

protected:
	void AddAllTags(UGameplayTagsManager& Manager);
	void AddTag(FGameplayTag& OutTag, const ANSICHAR* TagName, const ANSICHAR* TagComment);
 
private:
	static FSNGameplayTags GameplayTags;
};
