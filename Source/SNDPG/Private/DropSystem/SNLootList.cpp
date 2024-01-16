// Fill out your copyright notice in the Description page of Project Settings.


#include "DropSystem/SNLootList.h"


void USNLootList::RollForItemToDrop(TSubclassOf<ASNWorldCollectable>& OutItem, ESNLootSet_RollingForLootType RollType, float PlayerLevel, float EnemyLevel)
{
	TArray<FSNRegularLootList_LootList> OutTempDropList;
	for(int32 ItemIndex = 0; ItemIndex < ItemDropList.Num(); ++ItemIndex)
	{
		OutTempDropList.Add(ItemDropList[ItemIndex]);
	}
	
	ModifyWeightsBasedOnPlayerProgress(PlayerLevel, EnemyLevel, /*OUT*/ OutTempDropList);
	
	switch (RollType)
	{
	case ESNLootSet_RollingForLootType::None:
		break;
	case ESNLootSet_RollingForLootType::Random:
		OutItem = RandomRollForItem();
		break;
	case ESNLootSet_RollingForLootType::RandomWithWeight:
		OutItem = RandomWithWeightRollForItem(OutTempDropList);
		break;
	default:
		OutItem = RandomRollForItem();
		break;
	}
}

TSubclassOf<ASNWorldCollectable> USNLootList::RandomRollForItem()
{
	return ItemDropList[FMath::FRandRange<float>(0,ItemDropList.Num())].LootItem;
}

TSubclassOf<ASNWorldCollectable> USNLootList::RandomWithWeightRollForItem(TArray<FSNRegularLootList_LootList> InItemDropList)
{
	float WeightSum = 0.f;
	
	// Sum all the weights
	for(int32 ItemIndex = 0; ItemIndex < InItemDropList.Num(); ++ItemIndex)
	{
		WeightSum += InItemDropList[ItemIndex].ItemDropWeight;
	}

	float RandomNumber = FMath::FRandRange(0, WeightSum);

	// Loop over all the items until one is picked based on its weight (the higher the weight the more likely to get picked)
	for(int32 ItemIndex = 0; ItemIndex < InItemDropList.Num(); ++ItemIndex)
	{
		RandomNumber -= InItemDropList[ItemIndex].ItemDropWeight;

		if(RandomNumber <= 0)
		{
			return InItemDropList[ItemIndex].LootItem;
		}
	}
	
	return nullptr;
}

void USNLootList::ModifyWeightsBasedOnPlayerProgress(float PlayerLevel, float EnemyLevel, TArray<FSNRegularLootList_LootList>& TempItemDropList)
{
	//TODO Probably needs rethinking
	
	float LegendaryDropModifier = FMath::RoundToFloat(EnemyLevel * LegendaryPercentDropModifier);
	float EpicDropModifier = FMath::RoundToFloat(EnemyLevel * EpicPercentDropModifier);
	float CommonDropModifier = FMath::RoundToFloat(PlayerLevel * CommonPercentDropModifier);
	
	for(int32 ItemIndex = 0; ItemIndex < TempItemDropList.Num(); ++ItemIndex)
	{
		if (TempItemDropList[ItemIndex].ItemTier == ESNRegularLootList_ItemTier::Legendary)
		{
			TempItemDropList[ItemIndex].ItemDropWeight += LegendaryDropModifier;
		}
		else if(TempItemDropList[ItemIndex].ItemTier == ESNRegularLootList_ItemTier::Epic)
		{
			TempItemDropList[ItemIndex].ItemDropWeight += EpicDropModifier;
		}
		else if (TempItemDropList[ItemIndex].ItemTier == ESNRegularLootList_ItemTier::Common)
		{
			TempItemDropList[ItemIndex].ItemDropWeight -= CommonDropModifier;
		}
		else 
		{
			// We increase the Weight of NoDrop so Epic and Legendary items won't get too popular
			TempItemDropList[ItemIndex].ItemDropWeight += CommonDropModifier;
		}
	}
}

