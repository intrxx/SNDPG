// Copyright 2023 Michał Ogiński.


#include "InventorySystem/SNInventoryComponent.h"

#include "InventorySystem/SNItemBase.h"

USNInventoryComponent::USNInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	PrimaryComponentTick.bStartWithTickEnabled = false;
	
}

bool USNInventoryComponent::AddItem(USNItemBase* ItemToAdd)
{
	if(!ItemToAdd)
	{
		return false;
	}
	
	ItemToAdd->OwningInventory = this;
	ItemToAdd->World = GetWorld();
	Items.Add(ItemToAdd);

	OnInventoryUpdatedDelegate.Broadcast();

	return true;
}

bool USNInventoryComponent::RemoveItem(USNItemBase* ItemToRemove)
{
	if(!ItemToRemove)
	{
		return false;
	}
	
	ItemToRemove->OwningInventory = nullptr;
	ItemToRemove->World = nullptr;
	Items.RemoveSingle(ItemToRemove);
	
	OnInventoryUpdatedDelegate.Broadcast();

	return true;
}

void USNInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	for(auto& Item : DefaultItems)
	{
		AddItem(Item);
	}
}


