// Fill out your copyright notice in the Description page of Project Settings.


#include "DropSystem/SNBossLootList.h"

void USNBossLootList::FindBossItemToDrop(TSubclassOf<ASNWorldCollectable>& OutItem)
{
	OutItem = LootList[FMath::FRandRange<float>(0,LootList.Num())].LootItem;
}
