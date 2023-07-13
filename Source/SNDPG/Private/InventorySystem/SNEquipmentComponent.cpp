// Copyright 2023 Michał Ogiński.


#include "InventorySystem/SNEquipmentComponent.h"

#include "SWarningOrErrorBox.h"
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

		CurrentlyEquippedConsumable = EquippedConsumables.Last();
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

bool USNEquipmentComponent::SwitchEquippedConsumable(int16 Index)
{
	if(EquippedConsumables.IsEmpty())
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

void USNEquipmentComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

