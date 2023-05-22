// Copyright 2023 Michal Oginski.

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
	AddTag(Input_Look, "Input.Look", "Look input.");
	AddTag(Input_ToggleCharacterStatus, "Input.ToggleCharacterStatus", "Toggle input for Character Status Tab.");

	// Events
	AddTag(GameplayEvent_Death, "GameplayEvent.Death", "Event that fires on Death");

	// Abilities helper tags
	AddTag(Ability_Behavior_SurvivesDeath, "Ability.Behavior.SurvivesDeath", "Abilities with this tag will not be canceled upon death");
	
	// Ability Input
	AddTag(Ability_Input_Melee, "Ability.Input.Melee", "Melee ability input.");
	AddTag(Ability_Input_Healing, "Ability.Input.Healing", "Healing ability input.");

	// Ability Cooldown
	AddTag(Ability_CD_Healing, "Ability.CD.Healing", "Tag used for healing cooldown");

	// Notifiers Hit Tags
	AddTag(HitTag_HeroMelee, "Ability.Data.HitTag.HeroMelee", "Tag used by notify to trigger event.");
}

void FSNGameplayTags::AddTag(FGameplayTag& OutTag, const ANSICHAR* TagName, const ANSICHAR* TagComment)
{
	OutTag = UGameplayTagsManager::Get().AddNativeGameplayTag(FName(TagName), FString(TEXT("(Native) ")) + FString(TagComment));
}
