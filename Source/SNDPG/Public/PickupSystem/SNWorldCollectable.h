// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractableTarget.h"
#include "Pickupable.h"
#include "GameFramework/Actor.h"
#include "SNWorldCollectable.generated.h"

UCLASS()
class SNDPG_API ASNWorldCollectable : public AActor, public IInteractableTarget, public IPickupable
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASNWorldCollectable();
	
	virtual void GatherInteractionOptions(const FInteractionQuery& InteractQuery, FInteractionOptionBuilder& InteractionBuilder) override;
	virtual FInventoryPickup GetPickupInventory() const override;
	
protected:
	UPROPERTY(EditAnywhere, Category = "SN|Pickup")
	FInteractionOption Option;

	UPROPERTY(EditAnywhere, Category = "SN|Pickup")
	FInventoryPickup StaticInventory;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SN|Pickup", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* StaticMesh;
};
