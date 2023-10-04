// Fill out your copyright notice in the Description page of Project Settings.


#include "DropSystem/SNRegularLootList.h"
#include "InventorySystem/SNItemBase.h"

void USNRegularLootList::FindRegularItemToDrop(TSubclassOf<ASNWorldCollectable>& OutItem)
{
	OutItem = LootList[FMath::FRandRange<float>(0,LootList.Num())].LootItem;
}
