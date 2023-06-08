// Copyright 2023 Michal Oginski.


#include "GAS/Actors/SNProjectileBase.h"

#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
ASNProjectileBase::ASNProjectileBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));

}

// Called when the game starts or when spawned
void ASNProjectileBase::BeginPlay()
{
	Super::BeginPlay();
	
}

