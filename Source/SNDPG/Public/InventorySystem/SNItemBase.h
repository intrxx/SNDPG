// Copyright 2023 Michal Oginski.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SNItemBase.generated.h"

/**
 * 
 */
UCLASS(Abstract, BlueprintType, Blueprintable, EditInlineNew, DefaultToInstanced)
class SNDPG_API USNItemBase : public UObject
{
	GENERATED_BODY()

public:
	USNItemBase();
	
	virtual void Use(class ASNHero* Hero) PURE_VIRTUAL(USNItemBase, );

	UFUNCTION(BlueprintImplementableEvent, Category = "SN|Item")
	void OnUse(class ASNHero* Hero);

	virtual class UWorld* GetWorld() const {return World;};
	
public:
	UPROPERTY(Transient)
	class UWorld* World;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SN|Item")
	FText UseActionText;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SN|Item")
	class UStaticMesh* PickupMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SN|Item")
	class UTexture2D* Thumbnail;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SN|Item")
	FText ItemDisplayName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SN|Item", meta = (MultiLine = true))
	FText ItemDescription;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SN|Item", meta = (ClampMin = 0.0f))
	float Weight;

	UPROPERTY()
	class USNInventoryComponent* OwningInventory;

	
};