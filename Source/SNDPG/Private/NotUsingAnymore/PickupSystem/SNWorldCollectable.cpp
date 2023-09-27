// Fill out your copyright notice in the Description page of Project Settings.


#include "NotUsingAnymore/PickupSystem/SNWorldCollectable.h"


ASNWorldCollectable::ASNWorldCollectable()
{
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	SetRootComponent(StaticMesh);
}

void ASNWorldCollectable::GatherInteractionOptions(const FSNInteractionQuery& InteractQuery,
	FSNInteractionOptionBuilder& InteractionBuilder)
{
	InteractionBuilder.AddInteractionOption(Option);
}

FSNInventoryPickup ASNWorldCollectable::GetPickupInventory() const
{
	return StaticInventory;
}

