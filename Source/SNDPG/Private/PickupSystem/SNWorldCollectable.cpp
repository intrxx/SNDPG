// Fill out your copyright notice in the Description page of Project Settings.


#include "PickupSystem/SNWorldCollectable.h"
#include "Components/WidgetComponent.h"


ASNWorldCollectable::ASNWorldCollectable()
{
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	SetRootComponent(StaticMesh);

	ItemNameWidgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("ItemNameWidget"));
	ItemNameWidgetComp->SetupAttachment(RootComponent);
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

void ASNWorldCollectable::BeginPlay()
{
	Super::BeginPlay();
	
}

