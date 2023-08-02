// Copyright 2023 Michał Ogiński.

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
	FGameplayTag Input_ToggleInGameMenu;
	FGameplayTag Input_ItemSwitch_WeaponRightSlot;
	FGameplayTag Input_ItemSwitch_WeaponLeftSlot;
	FGameplayTag Input_ItemSwitch_ConsumableDownSlot;
	FGameplayTag Input_ItemSwitch_MagicUpSlot;
	FGameplayTag Input_UseConsumableItem;

	// Events
	FGameplayTag GameplayEvent_Death;

	// Abilities helper tags
	FGameplayTag Ability_Behavior_SurvivesDeath;
	FGameplayTag Ability_Behavior_BlockStaminaRegen;
	
	// Abilities
	FGameplayTag Ability_Input_LightMelee;
	FGameplayTag Ability_Input_HeavyMelee;
	FGameplayTag Ability_Input_Healing;
	FGameplayTag Ability_Input_Dash;
	FGameplayTag Ability_Input_TargetLock;
	FGameplayTag Ability_Input_UseWeaponSpell;
	FGameplayTag Ability_Input_Interact;

	// Ability tags
	FGameplayTag Ability_LightMelee;
	FGameplayTag Ability_HeavyMelee;
	FGameplayTag Ability_Healing;
	FGameplayTag Ability_SwirlSwing;
	FGameplayTag Ability_Dash;
	FGameplayTag Ability_KnifeThrow;
	FGameplayTag Ability_TargetLock;

	// Cooldowns
	FGameplayTag Ability_CD_Healing;
	FGameplayTag Ability_CD_ManaRestoring;
	FGameplayTag Ability_CD_SwirlSwing;
	FGameplayTag Ability_CD_KnifeThrow;
	FGameplayTag Ability_CD_Dash;

	// Data
	FGameplayTag HitTag_HeroMelee;

protected:
	void AddAllTags(UGameplayTagsManager& Manager);
	void AddTag(FGameplayTag& OutTag, const ANSICHAR* TagName, const ANSICHAR* TagComment);
 
private:
	static FSNGameplayTags GameplayTags;
};
