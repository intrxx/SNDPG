// Fill out your copyright notice in the Description page of Project Settings.


#include "DropSystem/SNLootList.h"
#include "InventorySystem/SNItemBase.h"

void USNLootList::RollForItemToDrop(TSubclassOf<ASNWorldCollectable>& OutItem, ESNLootSet_RollingForLootType RollType)
{
	OutItem = LootList[FMath::FRandRange<float>(0,LootList.Num())].LootItem;
}
