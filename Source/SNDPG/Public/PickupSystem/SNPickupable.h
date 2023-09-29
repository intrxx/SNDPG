// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InventorySystem/SNItemBase.h"
#include "SNPickupable.generated.h"

class USNInventoryComponent;

USTRUCT(BlueprintType)
struct FSNPickupableItem
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Instanced)
	TObjectPtr<USNItemBase> ItemDef;
};

USTRUCT(BlueprintType)
struct FSNInventoryPickup
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FSNPickupableItem> Instances;
};

// This class does not need to be modified.
UINTERFACE(MinimalAPI, BlueprintType, meta = (CannotImplementInterfaceInBlueprint))
class USNPickupable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SNDPG_API ISNPickupable
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	 virtual FSNInventoryPickup GetPickupInventory() const = 0;
};

/**  */
UCLASS()
class USNPickupableStatics : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	USNPickupableStatics();

public:
	UFUNCTION(BlueprintPure)
	static TScriptInterface<ISNPickupable> GetFirstPickupableFromActor(AActor* Actor);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, meta = (WorldContext = "Ability"))
	static void AddPickupToInventory(USNInventoryComponent* InventoryComponent, TScriptInterface<ISNPickupable> Pickup);
};
