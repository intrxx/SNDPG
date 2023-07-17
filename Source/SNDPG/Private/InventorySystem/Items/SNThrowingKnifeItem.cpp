// Copyright 2023 Michał Ogiński.


#include "InventorySystem/SNThrowingKnifeItem.h"

USNThrowingKnifeItem::USNThrowingKnifeItem()
{
	ItemCategory = EItemCategory::Consumable;
}

void USNThrowingKnifeItem::Use(ASNHero* Hero)
{
	Super::Use(Hero);
}
