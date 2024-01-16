// Copyright 2023 Michał Ogiński.

#pragma once

#include "CoreMinimal.h"
#include "UI/Inventory/SNEquipmentSlotButton.h"
#include "GAS/SNAbilitySet.h"
#include "SNItemBase.generated.h"

class USNGameplayAbility;
enum class ESlotCategory : uint8;

UENUM(BlueprintType)
enum class EItemCategory : uint8
{
	None,
	Weapon,
	Consumable,
	Helmet,
	BodyArmour,
	Gloves,
	Boots,
	Talisman,
	Magic,
	Arrow,
	Bolt
};

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
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SN|Item|Defaults")
	class UTexture2D* Thumbnail;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SN|Item|Defaults", meta = (ClampMin = 0.0f))
	float Weight;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SN|Item|Defaults")
	FText ItemDisplayName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SN|Item|Defaults", meta = (MultiLine = true))
	FText ItemDescription;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SN|Item|Defaults")
	FText UseActionText;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SN|Item|AbilitySet")
	USNAbilitySet* AbilitySet;

	UPROPERTY()
	FSNAbilitySet_GrantedHandles GrantedHandles;
	
	UPROPERTY()
	class USNInventoryComponent* OwningInventory;

	UPROPERTY()
	class USNEquipmentComponent* OwningEquipment;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SN|Item|Data")
	ESlotCategory SlotCategoryEquippedTo = ESlotCategory::None;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SN|Item|Data")
	bool bIsItemEquipped = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SN|Item|Data")
	UUserWidget* ItemEquipmentSlot;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SN|Item|Data")
	EItemCategory ItemCategory;
};
