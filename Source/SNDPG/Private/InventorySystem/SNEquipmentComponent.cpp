// Copyright 2023 Michał Ogiński.


#include "InventorySystem/SNEquipmentComponent.h"
#include "InventorySystem/SNItemBase.h"

USNEquipmentComponent::USNEquipmentComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	PrimaryComponentTick.bStartWithTickEnabled = false;
	
}

bool USNEquipmentComponent::AddToEquippedItems(USNItemBase* ItemToAdd)
{
	if(!ItemToAdd)
	{
		return false;
	}
	
	ItemToAdd->OwningEquipment = this;
	ItemToAdd->World = GetWorld();
	EquippedItems.Add(ItemToAdd);

	switch (ItemToAdd->ItemCategory)
	{
	case EItemCategory::LeftHandWeapon:
		EquippedLeftHandWeapon.Add(ItemToAdd);
		
		OnEquippedLeftHandWeaponUpdateDelegate.Broadcast();
		break;
		
	case EItemCategory::RightHandWeapon:
		EquippedRightHandWeapon.Add(ItemToAdd);
		
		OnEquippedRightHandWeaponUpdateDelegate.Broadcast();
		break;
		
	case EItemCategory::Consumable:
		EquippedConsumables.Add(ItemToAdd);
		
		OnEquippedConsumableUpdateDelegate.Broadcast();
		break;
		
	default:
		OnEquipmentUpdatedDelegate.Broadcast();
		break;
	}
	
	return true;
}

bool USNEquipmentComponent::RemoveFromEquippedItems(USNItemBase* ItemToRemove)
{
	if(!ItemToRemove)
	{
		return false;
	}

	ItemToRemove->OwningEquipment = nullptr;
	ItemToRemove->World = nullptr;
	EquippedItems.RemoveSingle(ItemToRemove);

	switch (ItemToRemove->ItemCategory)
	{
	case EItemCategory::LeftHandWeapon:
		EquippedLeftHandWeapon.RemoveSingle(ItemToRemove);
		
		OnEquippedLeftHandWeaponUpdateDelegate.Broadcast();
		break;
		
	case EItemCategory::RightHandWeapon:
		EquippedRightHandWeapon.RemoveSingle(ItemToRemove);

		OnEquippedRightHandWeaponUpdateDelegate.Broadcast();
		break;
		
	case EItemCategory::Consumable:
		EquippedConsumables.RemoveSingle(ItemToRemove);
		
		OnEquippedConsumableUpdateDelegate.Broadcast();
		break;
		
	default:
		OnEquipmentUpdatedDelegate.Broadcast();
		break;
	}
	
	
	return true;
}

void USNEquipmentComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

