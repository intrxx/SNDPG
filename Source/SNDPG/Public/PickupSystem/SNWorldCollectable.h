// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SNInteractableTarget.h"
#include "SNPickupable.h"
#include "SNInteractionOption.h"
#include "GameFramework/Actor.h"
#include "SNWorldCollectable.generated.h"

class UObject;
struct FSNInteractionQuery;

UCLASS(Abstract, Blueprintable)
class SNDPG_API ASNWorldCollectable : public AActor, public ISNInteractableTarget, public ISNPickupable
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASNWorldCollectable();
	
	virtual void GatherInteractionOptions(const FSNInteractionQuery& InteractQuery, FSNInteractionOptionBuilder& InteractionBuilder) override;
	virtual FSNInventoryPickup GetPickupInventory() const override;
	
protected:
	UPROPERTY(EditAnywhere, Category = "SN|Pickup")
	FSNInteractionOption Option;

	UPROPERTY(EditAnywhere, Category = "SN|Pickup")
	FSNInventoryPickup StaticInventory;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SN|Pickup", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* StaticMesh;
};
