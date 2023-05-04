// Copyright 2023 Michal Oginski.


#include "SNAssetManager.h"
#include "AbilitySystemGlobals.h"
#include "GameplayTags/SNGameplayTags.h"

USNAssetManager::USNAssetManager()
{
}

USNAssetManager& USNAssetManager::Get()
{
	check(GEngine)
	if(USNAssetManager* Singleton = Cast<USNAssetManager>(GEngine->AssetManager))
	{
		return *Singleton;
	}

	UE_LOG(LogTemp, Fatal, TEXT("Invalid AssetManagerClassName in DefaultEndine.ini. It must be set to GAssetManager!"))

	return *NewObject<USNAssetManager>();
}

void USNAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();
	
	FSNGameplayTags::InitializeNativeTags();
	UAbilitySystemGlobals::Get().InitGlobalData();
}
