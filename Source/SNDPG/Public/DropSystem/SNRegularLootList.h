// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SNLootSet.h"
#include "Engine/DataAsset.h"
#include "SNRegularLootList.generated.h"

class ASNWorldCollectable;

UENUM(BlueprintType)
enum class ESNRegularLootList_ItemTier : uint8
{
	Common,
	Rare,
	Legendary
};


USTRUCT(BlueprintType)
struct FSNRegularLootList_LootList
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ASNWorldCollectable> LootItem;

	UPROPERTY(EditDefaultsOnly)
	ESNRegularLootList_ItemTier ItemTier;

	UPROPERTY(EditDefaultsOnly)
	float RequiredItemLevel = 0.0f;
};

/**
 * 
 */
UCLASS(BlueprintType)
class SNDPG_API USNRegularLootList : public USNLootSet
{
	GENERATED_BODY()
	
public:
	void FindRegularItemToDrop(TSubclassOf<ASNWorldCollectable>& OutItem, ESNLootSet_RollingForLootType RollType);

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Loot List")
	TArray<FSNRegularLootList_LootList> LootList;
};
