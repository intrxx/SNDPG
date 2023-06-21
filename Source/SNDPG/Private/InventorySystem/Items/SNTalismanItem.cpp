// Fill out your copyright notice in the Description page of Project Settings.


#include "InventorySystem/Items/SNTalismanItem.h"

USNTalismanItem::USNTalismanItem()
{
	ItemCategory = EItemCategory::Talisman;
}

void USNTalismanItem::Use(ASNHero* Hero)
{
	Super::Use(Hero);
}
