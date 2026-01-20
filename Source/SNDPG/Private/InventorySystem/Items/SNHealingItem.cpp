// Copyright 2026 out of sCope team - intrxx


#include "InventorySystem/Items/SNHealingItem.h"

USNHealingItem::USNHealingItem()
{
	ItemCategory = EItemCategory::Consumable;
}

void USNHealingItem::Use(ASNHero* Hero)
{
	Super::Use(Hero);
	
}
