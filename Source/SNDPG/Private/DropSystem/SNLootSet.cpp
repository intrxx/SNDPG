// Fill out your copyright notice in the Description page of Project Settings.


#include "DropSystem/SNLootSet.h"

#include "DropSystem/SNBossLootList.h"
#include "DropSystem/SNRegularLootList.h"

void USNLootSet::FindItemToDrop(TSubclassOf<ASNWorldCollectable>& OutItem)
{
	if (IsValid(RegularLootList) && IsValid(BossLootList))
	{
		if (FMath::RandBool())
		{
			BossLootList->FindBossItemToDrop(OutItem);
			return;
		}
	}
	
	if(IsValid(RegularLootList))
	{
		RegularLootList->FindRegularItemToDrop(OutItem);
		return;
	}

	if (IsValid(BossLootList))
	{
		BossLootList->FindBossItemToDrop(OutItem);
	}
}
