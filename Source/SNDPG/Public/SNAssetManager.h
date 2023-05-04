// Copyright 2023 Michal Oginski.

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "SNAssetManager.generated.h"

/**
 * 
 */
UCLASS()
class SNDPG_API USNAssetManager : public UAssetManager
{
	GENERATED_BODY()

public:
	USNAssetManager();

	static USNAssetManager& Get();

	virtual void StartInitialLoading() override;
	
};
