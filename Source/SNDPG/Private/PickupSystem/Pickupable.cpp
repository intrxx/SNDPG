// Fill out your copyright notice in the Description page of Project Settings.


#include "PickupSystem/Pickupable.h"
#include "GameFramework/Actor.h"
#include "InventorySystem/SNInventoryComponent.h"
#include "UObject/ScriptInterface.h"

// Add default functionality here for any IPickupable functions that are not pure virtual.
UPickupableStatics::UPickupableStatics()
{
}

TScriptInterface<IPickupable> UPickupableStatics::GetFirstPickupableFromActor(AActor* Actor)
{
	// If the actor is directly pickupable, return that.
	TScriptInterface<IPickupable> PickupableActor(Actor);
	if (PickupableActor)
	{
		return PickupableActor;
	}

	// If the actor isn't pickupable, it might have a component that has a pickupable interface.
	TArray<UActorComponent*> PickupableComponents = Actor ? Actor->GetComponentsByInterface(UPickupable::StaticClass()) : TArray<UActorComponent*>();
	if (PickupableComponents.Num() > 0)
	{
		// Get first pickupable, if the user needs more sophisticated pickup distinction, will need to be solved elsewhere.
		return TScriptInterface<IPickupable>(PickupableComponents[0]);
	}

	return TScriptInterface<IPickupable>();
}

void UPickupableStatics::AddPickupToInventory(USNInventoryComponent* InventoryComponent,
	TScriptInterface<IPickupable> Pickup)
{
	if(Pickup && InventoryComponent)
	{
		const FInventoryPickup& PickupInventory = Pickup->GetPickupInventory();

		for(const FPickupableItem& Instances : PickupInventory.Instances)
		{
			InventoryComponent->AddItem(Instances.ItemDef);
		}
	}
}
