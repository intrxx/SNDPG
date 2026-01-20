// Copyright 2026 out of sCope team - intrxx


#include "InventorySystem/SNThrowingKnifeItem.h"

USNThrowingKnifeItem::USNThrowingKnifeItem()
{
	ItemCategory = EItemCategory::Consumable;
}

void USNThrowingKnifeItem::Use(ASNHero* Hero)
{
	Super::Use(Hero);
}
