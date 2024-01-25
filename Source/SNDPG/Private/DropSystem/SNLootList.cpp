// Fill out your copyright notice in the Description page of Project Settings.


#include "DropSystem/SNLootList.h"

#include "DSP/AudioDebuggingUtilities.h"


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

void USNLootList::ModifyWeightsBasedOnPlayerProgress(float PlayerLevel, float EnemyLevel,
	TArray<FSNRegularLootList_LootList>& TempItemDropList)
{
	float NewCommonWeight;
	float NewEpicWeight;
	float NewLegendaryWeight;
	
	for(int32 ItemIndex = 0; ItemIndex < TempItemDropList.Num(); ++ItemIndex)
	{
		switch (TempItemDropList[ItemIndex].ItemTier)
		{
		case ESNRegularLootList_ItemTier::Common:
			NewCommonWeight = (0.62 * (TempItemDropList[ItemIndex].ItemDropWeight - EnemyLevel)) * 1.98;
			
			TempItemDropList[ItemIndex].ItemDropWeight = FMath::Clamp(NewCommonWeight, 0.0f,
				TempItemDropList[ItemIndex].ItemDropWeight);
			break;
		case ESNRegularLootList_ItemTier::Epic:
			NewEpicWeight = (0.52 * (TempItemDropList[ItemIndex].ItemDropWeight + PlayerLevel)) * 1.88;
			
			TempItemDropList[ItemIndex].ItemDropWeight = FMath::Max(NewEpicWeight,
				TempItemDropList[ItemIndex].ItemDropWeight);
			break;
		case ESNRegularLootList_ItemTier::Legendary:
			NewLegendaryWeight = (0.52 * (TempItemDropList[ItemIndex].ItemDropWeight + PlayerLevel)) * 1.55;
			
			TempItemDropList[ItemIndex].ItemDropWeight = FMath::Max(NewLegendaryWeight,
				TempItemDropList[ItemIndex].ItemDropWeight);
			break;
		case ESNRegularLootList_ItemTier::NoDrop:
			TempItemDropList[ItemIndex].ItemDropWeight += 5 * (int32)PlayerLevel % 10;
			break;
		}
	}
}

