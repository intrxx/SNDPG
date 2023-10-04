// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "SNLootSet.generated.h"

class USNBossLootList;
class USNRegularLootList;
class ASNWorldCollectable;
/**
 * 
 */
UCLASS(BlueprintType)
class SNDPG_API USNLootSet : public UDataAsset
{
	GENERATED_BODY()

public:
	void FindItemToDrop(TSubclassOf<ASNWorldCollectable>& OutItem);

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SN|Loot")
	TObjectPtr<USNBossLootList> BossLootList;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SN|Loot")
	TObjectPtr<USNRegularLootList> RegularLootList;
	
};
