// Fill out your copyright notice in the Description page of Project Settings.


#include "DropSystem/SNLootSet.h"
#include "DropSystem/SNLootList.h"

void USNLootSet::ModifyWeightsBasedOnPlayerProgress(float PlayerLevel, float EnemyLevel, TObjectPtr<USNLootList> LootList)
{
	
}

void USNLootSet::FindItemToDrop(TSubclassOf<ASNWorldCollectable>& OutItem)
{
	if (IsValid(RegularLootList) && IsValid(BossLootList))
	{
		if (FMath::RandBool())
		{
			ModifyWeightsBasedOnPlayerProgress(HeroCharacterLevel, EnemyCharacterLevel, BossLootList);
			BossLootList->RollForItemToDrop(OutItem, RollingForLootType);
			return;
		}
	}
	
	if(IsValid(RegularLootList))
	{
		RegularLootList->RollForItemToDrop(OutItem, RollingForLootType);
		return;
	}

	if (IsValid(BossLootList))
	{
		BossLootList->RollForItemToDrop(OutItem, RollingForLootType);
	}
}

void USNLootSet::FillTheEnemyCharacterData(float Level)
{
	EnemyCharacterLevel = Level;
}

void USNLootSet::FillTheHeroCharacterData(float Level)
{
	HeroCharacterLevel = Level;
}
