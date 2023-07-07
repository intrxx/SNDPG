// Fill out your copyright notice in the Description page of Project Settings.


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

	OnEquipmentUpdatedDelegate.Broadcast();
	
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

	OnEquipmentUpdatedDelegate.Broadcast();
	
	return true;
}

void USNEquipmentComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

