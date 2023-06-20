// Fill out your copyright notice in the Description page of Project Settings.


#include "InventorySystem/Items/SNHealingItem.h"

USNHealingItem::USNHealingItem()
{
	ItemCategory = EItemCategory::Consumable;
}

void USNHealingItem::Use(ASNHero* Hero)
{
	Super::Use(Hero);
	
}
