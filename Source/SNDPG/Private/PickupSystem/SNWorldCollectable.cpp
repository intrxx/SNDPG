// Fill out your copyright notice in the Description page of Project Settings.


#include "PickupSystem/SNWorldCollectable.h"


ASNWorldCollectable::ASNWorldCollectable()
{
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(RootComponent);
}

void ASNWorldCollectable::GatherInteractionOptions(const FInteractionQuery& InteractQuery,
	FInteractionOptionBuilder& InteractionBuilder)
{
	InteractionBuilder.AddInteractionOption(Option);
}

FInventoryPickup ASNWorldCollectable::GetPickupInventory() const
{
	return StaticInventory;
}

