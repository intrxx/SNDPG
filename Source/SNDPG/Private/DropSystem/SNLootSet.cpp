// Fill out your copyright notice in the Description page of Project Settings.


#include "DropSystem/SNLootSet.h"
#include "DropSystem/SNLootList.h"

void USNLootSet::FindItemToDrop(TSubclassOf<ASNWorldCollectable>& OutItem)
{
	if (IsValid(RegularLootList) && IsValid(BossLootList))
	{
		if (FMath::RandRange(1, 100) <= 35)
		{
			BossLootList->RollForItemToDrop(OutItem, RollingForLootType, HeroCharacterLevel, EnemyCharacterLevel);
			return;
		}
	}

	if(IsValid(RegularLootList))
	{
		RegularLootList->RollForItemToDrop(OutItem, RollingForLootType, HeroCharacterLevel, EnemyCharacterLevel);
		return;
	}

	if (IsValid(BossLootList))
	{
		BossLootList->RollForItemToDrop(OutItem, RollingForLootType, HeroCharacterLevel, EnemyCharacterLevel);
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
