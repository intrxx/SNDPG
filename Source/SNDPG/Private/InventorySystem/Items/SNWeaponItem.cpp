// Fill out your copyright notice in the Description page of Project Settings.


#include "InventorySystem/Items/SNWeaponItem.h"

USNWeaponItem::USNWeaponItem()
{
	//ItemCategory = EItemCategory::Weapon;
}

void USNWeaponItem::Use(ASNHero* Hero)
{
	Super::Use(Hero);
}
