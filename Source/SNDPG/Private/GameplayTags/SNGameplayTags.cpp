// Copyright 2023 Michał Ogiński.

#include "GameplayTags/SNGameplayTags.h"
#include "GameplayTagsManager.h"

FSNGameplayTags FSNGameplayTags::GameplayTags;

void FSNGameplayTags::InitializeNativeTags()
{
	UGameplayTagsManager& Manager = UGameplayTagsManager::Get();

	GameplayTags.AddAllTags(Manager);

	Manager.DoneAddingNativeTags();
}

FGameplayTag FSNGameplayTags::FindTagByString(FString TagString, bool bMatchPartialString)
{
	const UGameplayTagsManager& Manager = UGameplayTagsManager::Get();
	FGameplayTag Tag = Manager.RequestGameplayTag(FName(*TagString), false);

	if(!Tag.IsValid() && bMatchPartialString)
	{
		FGameplayTagContainer AllTags;
		Manager.RequestAllGameplayTags(AllTags, true);

		for (const FGameplayTag TestTag : AllTags)
		{
			if(TestTag.ToString().Contains(TagString))
			{
				UE_LOG(LogTemp, Display, TEXT("Could not find exact match for tag [%s] but found partialo match on tag [%s]."), *TagString, *TestTag.ToString());
				Tag = TestTag;
				break;
			}
		}
	}
	return Tag;
}

void FSNGameplayTags::AddAllTags(UGameplayTagsManager& Manager)
{
	// Native Input
	AddTag(Input_Move, "Input.Move", "Move input.");
	AddTag(Input_Look_Mouse, "Input.Look.Mouse", "Look input with mouse.");
	AddTag(Input_Look_Stick, "Input.Look.Stick", "Look input with stick.");
	AddTag(Input_ToggleInGameMenu, "Input.ToggleInGameMenu", "Toggle input for In Game Menu Tab.");

	// Events
	AddTag(GameplayEvent_Death, "GameplayEvent.Death", "Event that fires on Death");

	// Abilities helper tags
	AddTag(Ability_Behavior_SurvivesDeath, "Ability.Behavior.SurvivesDeath", "Abilities with this tag will not be canceled upon death");
	AddTag(Ability_Behavior_BlockStaminaRegen, "Ability.Behavior.BlockStaminaRegen", "Tag used to block stamina regen while attacking");
	
	// Ability Input
	AddTag(Ability_Input_LightMelee, "Ability.Input.Melee.Light", "Melee ability input.");
	AddTag(Ability_Input_HeavyMelee, "Ability.Input.Melee.Heavy", "Melee ability input.");
	AddTag(Ability_Input_Healing, "Ability.Input.Healing", "Healing ability input.");
	AddTag(Ability_Input_Dash, "Ability.Input.Dash", "Dash ability input.");
	AddTag(Ability_Input_Swirl, "Ability.Input.Swirl", "Swirl ability input.");
	AddTag(Ability_Input_KnifeThrow, "Ability.Input.KnifeThrow", "Knife Throw ability input.");
	AddTag(Ability_Input_TargetLock, "Ability.Input.TargetLock", "Target Lock ability input.");

	// Ability tags
	AddTag(Ability_Dash, "Ability.Owned.Dash", "Dash owned tag.");
	AddTag(Ability_Healing, "Ability.Owned.Healing", "Healing owned tag.");
	AddTag(Ability_HeavyMelee, "Ability.Owned.HeavyMelee", "HeavyMelee owned tag.");
	AddTag(Ability_LightMelee, "Ability.Owned.LightMelee", "LightMelee owned tag.");
	AddTag(Ability_KnifeThrow, "Ability.Owned.KnifeThrow", "KnifeThrow owned tag.");
	AddTag(Ability_SwirlSwing, "Ability.Owned.SwirlSwing", "SwirlSwing owned tag.");
	AddTag(Ability_TargetLock, "Ability.Owned.TargetLock", "TargetLock owned tag.");

	// Ability Cooldown
	AddTag(Ability_CD_Healing, "Ability.CD.Healing", "Tag used for healing cooldown");
	AddTag(Ability_CD_ManaRestoring, "Ability.CD.ManaRestoring", "Tag used for Mana Restoring cooldown");
	AddTag(Ability_CD_SwirlSwing, "Ability.CD.SwirlSwing", "Tag used for Swirl Swing cooldown");
	AddTag(Ability_CD_Dash, "Ability.CD.Dash", "Tag used for Dash cooldown");
	AddTag(Ability_CD_KnifeThrow, "Ability.CD.KnifeThrow", "Tag used for KnifeThrow cooldown");

	// Notifiers Hit Tags
	AddTag(HitTag_HeroMelee, "Ability.Data.HitTag.HeroMelee", "Tag used by notify to trigger event.");
}

void FSNGameplayTags::AddTag(FGameplayTag& OutTag, const ANSICHAR* TagName, const ANSICHAR* TagComment)
{
	OutTag = UGameplayTagsManager::Get().AddNativeGameplayTag(FName(TagName), FString(TEXT("(Native) ")) + FString(TagComment));
}
