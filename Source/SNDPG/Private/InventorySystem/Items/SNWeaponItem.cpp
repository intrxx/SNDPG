// Copyright 2023 Michał Ogiński.


#include "InventorySystem/Items/SNWeaponItem.h"

USNWeaponItem::USNWeaponItem()
{
	ItemCategory = EItemCategory::Weapon;
}

void USNWeaponItem::Use(ASNHero* Hero)
{
	Super::Use(Hero);
}
