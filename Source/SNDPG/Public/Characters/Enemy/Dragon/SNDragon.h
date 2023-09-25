// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/Enemy/SNEnemy.h"
#include "SNDragon.generated.h"

/**
 * 
 */
UCLASS()
class SNDPG_API ASNDragon : public ASNEnemy
{
	GENERATED_BODY()

public:
	ASNDragon(const FObjectInitializer& ObjectInitializer);
};
