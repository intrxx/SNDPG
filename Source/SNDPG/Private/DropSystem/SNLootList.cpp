// Fill out your copyright notice in the Description page of Project Settings.


#include "DropSystem/SNLootList.h"

void USNLootList::RollForItemToDrop(TSubclassOf<ASNWorldCollectable>& OutItem, ESNLootSet_RollingForLootType RollType)
{
	switch (RollType)
	{
	case ESNLootSet_RollingForLootType::None:
		break;
	case ESNLootSet_RollingForLootType::Random:
		OutItem = RandomRollForItem();
		break;
	case ESNLootSet_RollingForLootType::RandomWithWeight:
		break;
	default:
		OutItem = RandomRollForItem();
		break;
	}
}

TSubclassOf<ASNWorldCollectable> USNLootList::RandomRollForItem()
{
	return LootList[FMath::FRandRange<float>(0,LootList.Num())].LootItem;
}

TSubclassOf<ASNWorldCollectable> USNLootList::RandomWithWeightRollForItem()
{
	
}

