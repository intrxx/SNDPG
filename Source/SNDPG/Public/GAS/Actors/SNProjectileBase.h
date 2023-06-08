// Copyright 2023 Michal Oginski.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
#include "GameFramework/Actor.h"
#include "SNProjectileBase.generated.h"

UCLASS()
class SNDPG_API ASNProjectileBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASNProjectileBase();

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float Range;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	class UProjectileMovementComponent* ProjectileMovement;

	UPROPERTY(BlueprintReadWrite, Meta = (ExposeOnSpawn = true))
	FGameplayEffectSpecHandle DamageEffectSpecHandle;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
};
