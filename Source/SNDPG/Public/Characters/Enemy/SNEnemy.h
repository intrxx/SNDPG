// Copyright 2023 Michal Oginski.

#pragma once

#include "CoreMinimal.h"
#include "Characters/SNCharacterBase.h"
#include "SNEnemy.generated.h"

/**
 * 
 */
UCLASS()
class SNDPG_API ASNEnemy : public ASNCharacterBase
{
	GENERATED_BODY()

public:
	ASNEnemy(const FObjectInitializer& ObjectInitializer);
};
