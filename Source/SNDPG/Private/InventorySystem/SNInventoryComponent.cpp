// Copyright 2023 Michal Oginski.


#include "InventorySystem/SNInventoryComponent.h"

#include "InventorySystem/SNItemBase.h"

USNInventoryComponent::USNInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	PrimaryComponentTick.bStartWithTickEnabled = false;

	InventoryCapacity = 30.0f;
	
}

bool USNInventoryComponent::AddItem(USNItemBase* ItemToAdd)
{
	if(Items.Num() >= InventoryCapacity || !ItemToAdd)
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
	if(ItemToRemove)
	{
		ItemToRemove->OwningInventory = nullptr;
		ItemToRemove->World = nullptr;
		Items.RemoveSingle(ItemToRemove);
		OnInventoryUpdatedDelegate.Broadcast();

		return true;
	}
	return false;
}

void USNInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	for(auto& Item : DefaultItems)
	{
		AddItem(Item);
	}
}


