// Copyright 2026 out of sCope team - intrxx


#include "InventorySystem/Items/SNWeaponItem.h"

USNWeaponItem::USNWeaponItem()
{
	ItemCategory = EItemCategory::Weapon;
}

void USNWeaponItem::Use(ASNHero* Hero)
{
	Super::Use(Hero);
}
