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
		OutItem = RandomWithWeightRollForItem();
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
	float WeightSum = 0;
	
	// Sum all the weights
	for(int32 ItemIndex = 0; ItemIndex < LootList.Num(); ++ItemIndex)
	{
		WeightSum += LootList[ItemIndex].ItemDropWeight;
	}
	
	float RandomNumber = FMath::FRandRange(0, WeightSum);

	// Loop through all the items until one is picked based on its weight (the higher the weight the more likely to get picked)
	for(int32 ItemIndex = 0; ItemIndex < LootList.Num(); ++ItemIndex)
	{
		RandomNumber -= LootList[ItemIndex].ItemDropWeight;

		if(RandomNumber <= 0)
		{
			return LootList[ItemIndex].LootItem;
		}
	}
	
	return nullptr;
}

