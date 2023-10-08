// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SNLootSet.h"
#include "Engine/DataAsset.h"
#include "SNBossLootList.generated.h"

class ASNWorldCollectable;

USTRUCT(BlueprintType)
struct FSNBossLootList_LootList
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ASNWorldCollectable> LootItem;
};

/**
 * 
 */
UCLASS(BlueprintType)
class SNDPG_API USNBossLootList : public USNLootSet
{
	GENERATED_BODY()

public:
	void FindBossItemToDrop(TSubclassOf<ASNWorldCollectable>& OutItem, ESNLootSet_RollingForLootType RollType);

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Loot List")
	TArray<FSNBossLootList_LootList> LootList;
};
