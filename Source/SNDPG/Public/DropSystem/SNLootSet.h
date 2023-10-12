// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "SNLootSet.generated.h"

class USNLootList;
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
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Loot Lists")
	TObjectPtr<USNLootList> BossLootList;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Loot Lists")
	TObjectPtr<USNLootList> RegularLootList;

	UPROPERTY(EditDefaultsOnly, Category = "Loot rolling type")
	ESNLootSet_RollingForLootType RollingForLootType = ESNLootSet_RollingForLootType::None;

protected:
	float EnemyCharacterLevel;
	float HeroCharacterLevel;
	
};
