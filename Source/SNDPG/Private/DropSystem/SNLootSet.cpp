// Fill out your copyright notice in the Description page of Project Settings.


#include "DropSystem/SNLootSet.h"
#include "DropSystem/SNBossLootList.h"
#include "DropSystem/SNRegularLootList.h"

void USNLootSet::ModifyWeightsBasedOnPlayerProgress(float PlayerLevel, float EnemyLevel, TObjectPtr<TSubclassOf<UDataAsset>> LootList)
{
	
}

void USNLootSet::FindItemToDrop(TSubclassOf<ASNWorldCollectable>& OutItem)
{
	if (IsValid(RegularLootList) && IsValid(BossLootList))
	{
		if (FMath::RandBool())
		{
			ModifyWeightsBasedOnPlayerProgress(HeroCharacterLevel, EnemyCharacterLevel, BossLootList);
			BossLootList->FindBossItemToDrop(OutItem, RollingForLootType);
			return;
		}
	}
	
	if(IsValid(RegularLootList))
	{
		RegularLootList->FindRegularItemToDrop(OutItem, RollingForLootType);
		return;
	}

	if (IsValid(BossLootList))
	{
		BossLootList->FindBossItemToDrop(OutItem, RollingForLootType);
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
