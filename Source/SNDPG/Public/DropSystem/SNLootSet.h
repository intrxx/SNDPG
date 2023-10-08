// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "SNLootSet.generated.h"

class USNBossLootList;
class USNRegularLootList;
class ASNWorldCollectable;

UENUM(BlueprintType)
enum class ESNLootSet_RollingForLootType : uint8
{
	None,
	Random,
	RandomWithWeight
};

/**
 * 
 */
UCLASS(BlueprintType)
class SNDPG_API USNLootSet : public UDataAsset
{
	GENERATED_BODY()

public:
	void FindItemToDrop(TSubclassOf<ASNWorldCollectable>& OutItem);
	void FillTheEnemyCharacterData(float Level);
	void FillTheHeroCharacterData(float Level);

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SN|Loot")
	TObjectPtr<USNBossLootList> BossLootList;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SN|Loot")
	TObjectPtr<USNRegularLootList> RegularLootList;

	UPROPERTY(EditDefaultsOnly, Category = "Loot rolling type")
	ESNLootSet_RollingForLootType RollingForLootType = ESNLootSet_RollingForLootType::None;

protected:
	/**
	 * Before rolling for items we take into account:
	 * @param PlayerLevel We decrease the chance of dropping weaker items as the PlayerLevel gets bigger
	 * @param EnemyLevel We increase the chance of dropping stronger items as the EnemyLevel gets bigger
	 */
	void ModifyWeightsBasedOnPlayerProgress(float PlayerLevel, float EnemyLevel, TObjectPtr<TSubclassOf<UDataAsset>> LootList);

protected:
	float EnemyCharacterLevel;
	float HeroCharacterLevel;
	
};
