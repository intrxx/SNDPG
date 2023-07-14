// Copyright 2023 Michał Ogiński.


#include "InventorySystem/SNEquipmentComponent.h"
#include "InventorySystem/SNItemBase.h"

USNEquipmentComponent::USNEquipmentComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	PrimaryComponentTick.bStartWithTickEnabled = false;
	
}

bool USNEquipmentComponent::AddToEquippedItems(USNItemBase* ItemToAdd, ESlotCategory SlotCategory)
{
	if(!ItemToAdd)
	{
		return false;
	}
	
	ItemToAdd->OwningEquipment = this;
	ItemToAdd->World = GetWorld();
	EquippedItems.Add(ItemToAdd);

	switch (SlotCategory)
	{
	case ESlotCategory::LeftHandWeaponSlot:
		EquippedLeftHandWeapon.Add(ItemToAdd);

		CurrentlyEquippedLeftHandWeapon = EquippedLeftHandWeapon[0];
		OnEquippedLeftHandWeaponUpdateDelegate.Broadcast();
		break;
		
	case ESlotCategory::RightHandWeaponSlot:
		EquippedRightHandWeapon.Add(ItemToAdd);

		CurrentlyEquippedRightHandWeapon = EquippedRightHandWeapon[0];
		OnEquippedRightHandWeaponUpdateDelegate.Broadcast();
		break;
		
	case ESlotCategory::ConsumableSlot:
		EquippedConsumables.Add(ItemToAdd);
		UE_LOG(LogTemp, Warning, TEXT("Adding item to consumable slot: %s"), *ItemToAdd->GetName());
		
		CurrentlyEquippedConsumable = EquippedConsumables[0];
		OnEquippedConsumableUpdateDelegate.Broadcast();
		break;

	case ESlotCategory::ArmourSlot:
		//TODO Do something when we equip armour
		break;

	case ESlotCategory::TalismanSlot:
		//TODO Do something when we equip Talisman
		break;

	case ESlotCategory::MagicSlot:
		//TODO Do something when we equip Magic
		break;
		
	default:
		OnEquipmentUpdatedDelegate.Broadcast();
		break;
	}
	
	return true;
}

bool USNEquipmentComponent::RemoveFromEquippedItems(USNItemBase* ItemToRemove, ESlotCategory SlotCategory)
{
	if(!ItemToRemove)
	{
		return false;
	}

	ItemToRemove->OwningEquipment = nullptr;
	ItemToRemove->World = nullptr;
	EquippedItems.RemoveSingle(ItemToRemove);

	switch (SlotCategory)
	{
	case ESlotCategory::LeftHandWeaponSlot:
		EquippedLeftHandWeapon.RemoveSingle(ItemToRemove);

		if (!EquippedLeftHandWeapon.IsEmpty())
		{
			CurrentlyEquippedLeftHandWeapon = EquippedLeftHandWeapon[0];
		}
		else
		{
			CurrentlyEquippedLeftHandWeapon = nullptr;
		}
		
		OnEquippedLeftHandWeaponUpdateDelegate.Broadcast();
		break;
		
	case ESlotCategory::RightHandWeaponSlot:
		EquippedRightHandWeapon.RemoveSingle(ItemToRemove);

		if (!EquippedRightHandWeapon.IsEmpty())
		{
			CurrentlyEquippedRightHandWeapon = EquippedRightHandWeapon[0];
		}
		else
		{
			CurrentlyEquippedRightHandWeapon = nullptr;
		}

		OnEquippedRightHandWeaponUpdateDelegate.Broadcast();
		break;
		
	case ESlotCategory::ConsumableSlot:
		EquippedConsumables.RemoveSingle(ItemToRemove);
		UE_LOG(LogTemp, Warning, TEXT("Removing item from consumable slot: %s"), *ItemToRemove->GetName());

		if (!EquippedConsumables.IsEmpty())
		{
			CurrentlyEquippedConsumable = EquippedConsumables[0];
		}
		else
		{
			CurrentlyEquippedConsumable = nullptr;
		}
		
		OnEquippedConsumableUpdateDelegate.Broadcast();
		break;

	case ESlotCategory::ArmourSlot:
		//TODO Remove armour
		break;

	case ESlotCategory::TalismanSlot:
		//TODO Remove Talisman
		break;

	case ESlotCategory::MagicSlot:
		//TODO Remove Magic
		break;
		
	default:
		OnEquipmentUpdatedDelegate.Broadcast();
		break;
	}
	
	
	return true;
}

bool USNEquipmentComponent::SwitchEquippedConsumable(int16 Index)
{
	if(EquippedConsumables.IsEmpty() || EquippedConsumables.Num() == 1)
	{
		return false;
	}
	
	if(Index+1 < EquippedConsumables.Num())
	{
		CurrentlyEquippedConsumable = EquippedConsumables[Index+1];
		CurrentlyEquippedConsumableIndex = Index+1;

		UE_LOG(LogTemp, Warning, TEXT("Curentely Equipped Consumable: %s"), *CurrentlyEquippedConsumable->GetName());
		return true;
	}

	if(Index+1 == EquippedConsumables.Num())
	{
		CurrentlyEquippedConsumable = EquippedConsumables[0];
		CurrentlyEquippedConsumableIndex = 0;

		UE_LOG(LogTemp, Warning, TEXT("Curentely Equipped Consumable: %s"), *CurrentlyEquippedConsumable->GetName());
		return true;
	}
	
	return false;
}

bool USNEquipmentComponent::SwitchEquippedLeftHandWeapon(int16 Index)
{
	if(EquippedLeftHandWeapon.IsEmpty() || EquippedLeftHandWeapon.Num() == 1)
	{
		return false;
	}
	
	if(Index+1 < EquippedLeftHandWeapon.Num())
	{
		CurrentlyEquippedLeftHandWeapon = EquippedLeftHandWeapon[Index+1];
		CurrentlyEquippedLeftHandWeaponIndex = Index+1;

		UE_LOG(LogTemp, Warning, TEXT("Curentely Equipped Consumable: %s"), *CurrentlyEquippedLeftHandWeapon->GetName());
		return true;
	}

	if(Index+1 == EquippedLeftHandWeapon.Num())
	{
		CurrentlyEquippedLeftHandWeapon = EquippedLeftHandWeapon[0];
		CurrentlyEquippedLeftHandWeaponIndex = 0;

		UE_LOG(LogTemp, Warning, TEXT("Curentely Equipped Consumable: %s"), *CurrentlyEquippedLeftHandWeapon->GetName());
		return true;
	}
	
	return false;
}

bool USNEquipmentComponent::SwitchEquippedRightHandWeapon(int16 Index)
{
	if(EquippedRightHandWeapon.IsEmpty() || EquippedRightHandWeapon.Num() == 1)
	{
		return false;
	}
	
	if(Index+1 < EquippedRightHandWeapon.Num())
	{
		CurrentlyEquippedRightHandWeapon = EquippedRightHandWeapon[Index+1];
		CurrentlyEquippedRightHandWeaponIndex = Index+1;

		UE_LOG(LogTemp, Warning, TEXT("Curentely Equipped Consumable: %s"), *CurrentlyEquippedRightHandWeapon->GetName());
		return true;
	}

	if(Index+1 == EquippedRightHandWeapon.Num())
	{
		CurrentlyEquippedRightHandWeapon = EquippedRightHandWeapon[0];
		CurrentlyEquippedRightHandWeaponIndex = 0;

		UE_LOG(LogTemp, Warning, TEXT("Curentely Equipped Consumable: %s"), *CurrentlyEquippedRightHandWeapon->GetName());
		return true;
	}
	
	return false;
}

bool USNEquipmentComponent::SwitchEquippedMagic(int16 Index)
{
	if(EquippedMagic.IsEmpty() || EquippedMagic.Num() == 1)
	{
		return false;
	}
	
	if(Index+1 < EquippedMagic.Num())
	{
		CurrentlyEquippedMagic = EquippedMagic[Index+1];
		CurrentlyEquippedMagicIndex = Index+1;

		UE_LOG(LogTemp, Warning, TEXT("Curentely Equipped Consumable: %s"), *CurrentlyEquippedMagic->GetName());
		return true;
	}

	if(Index+1 == EquippedMagic.Num())
	{
		CurrentlyEquippedMagic = EquippedMagic[0];
		CurrentlyEquippedMagicIndex = 0;

		UE_LOG(LogTemp, Warning, TEXT("Curentely Equipped Consumable: %s"), *CurrentlyEquippedMagic->GetName());
		return true;
	}
	
	return false;
}

void USNEquipmentComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

