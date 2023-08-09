// Fill out your copyright notice in the Description page of Project Settings.


#include "..\..\Public\PickupSystem\SNPickupable.h"
#include "GameFramework/Actor.h"
#include "InventorySystem/SNInventoryComponent.h"
#include "UObject/ScriptInterface.h"

// Add default functionality here for any IPickupable functions that are not pure virtual.
USNPickupableStatics::USNPickupableStatics()
{
}

TScriptInterface<ISNPickupable> USNPickupableStatics::GetFirstPickupableFromActor(AActor* Actor)
{
	// If the actor is directly pickupable, return that.
	TScriptInterface<ISNPickupable> PickupableActor(Actor);
	if (PickupableActor)
	{
		return PickupableActor;
	}

	// If the actor isn't pickupable, it might have a component that has a pickupable interface.
	TArray<UActorComponent*> PickupableComponents = Actor ? Actor->GetComponentsByInterface(USNPickupable::StaticClass()) : TArray<UActorComponent*>();
	if (PickupableComponents.Num() > 0)
	{
		// Get first pickupable, if the user needs more sophisticated pickup distinction, will need to be solved elsewhere.
		return TScriptInterface<ISNPickupable>(PickupableComponents[0]);
	}

	return TScriptInterface<ISNPickupable>();
}

void USNPickupableStatics::AddPickupToInventory(USNInventoryComponent* InventoryComponent,
	TScriptInterface<ISNPickupable> Pickup)
{
	if(Pickup && InventoryComponent)
	{
		const FSNInventoryPickup& PickupInventory = Pickup->GetPickupInventory();

		for(const FSNPickupableItem& Instances : PickupInventory.Instances)
		{
			InventoryComponent->AddItem(Instances.ItemDef);
		}
	}
}
