// Copyright 2026 out of sCope team - intrxx


#include "InventorySystem/Items/SNTalismanItem.h"

USNTalismanItem::USNTalismanItem()
{
	ItemCategory = EItemCategory::Talisman;
}

void USNTalismanItem::Use(ASNHero* Hero)
{
	Super::Use(Hero);
}
