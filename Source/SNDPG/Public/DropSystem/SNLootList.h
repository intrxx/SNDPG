// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SNLootSet.h"
#include "Engine/DataAsset.h"
#include "SNLootList.generated.h"

class ASNWorldCollectable;

UENUM(BlueprintType)
enum class ESNRegularLootList_ItemTier : uint8
{
	NoDrop,
	Common,
	Epic,
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
	float ItemDropWeight = 0.0f;
};

/**
 * 
 */
UCLASS(BlueprintType)
class SNDPG_API USNLootList : public UDataAsset
{
	GENERATED_BODY()
	
public:
	void RollForItemToDrop(TSubclassOf<ASNWorldCollectable>& OutItem, ESNLootSet_RollingForLootType RollType, float PlayerLevel, float EnemyLevel);

	/**
	 * Before rolling for items we take into account:
	 * @param PlayerLevel We decrease the chance of dropping weaker items as the PlayerLevel gets bigger,
	 * @param EnemyLevel We increase the chance of dropping stronger items as the EnemyLevel gets bigger.
	 * @param TempItemDropList Temporary array that holds all the items with Item Weights to modify.
	 * Modifiers are additive.
	 */
	void ModifyWeightsBasedOnPlayerProgress(float PlayerLevel, float EnemyLevel, TArray<FSNRegularLootList_LootList>& TempItemDropList);

protected:
	TSubclassOf<ASNWorldCollectable> RandomRollForItem();
	TSubclassOf<ASNWorldCollectable> RandomWithWeightRollForItem(TArray<FSNRegularLootList_LootList> TempItemDropList);

protected:
	UPROPERTY(EditDefaultsOnly, Category = "No Drop Weight")
	float NoDropWeight = 100.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Drop Modifier")
	float LegendaryPercentDropModifier = 0.025f;
	
	UPROPERTY(EditDefaultsOnly, Category = "Drop Modifier")
	float EpicPercentDropModifier = 0.05f;
	
	UPROPERTY(EditDefaultsOnly, Category = "Drop Modifier")
	float CommonPercentDropModifier = 0.25f;

	UPROPERTY(EditDefaultsOnly, Category = "Drop Modifier")
	float ConstLevelDifference = 2.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Loot List")
	TArray<FSNRegularLootList_LootList> ItemDropList;
	
};
