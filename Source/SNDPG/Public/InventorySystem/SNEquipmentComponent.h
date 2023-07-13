// Copyright 2023 Michał Ogiński.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SNEquipmentComponent.generated.h"

enum class EItemCategory : uint8;
class USNItemBase;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEquipmentUpdate);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEquippedConsumableUpdate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEquippedRightHandWeaponUpdate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEquippedLeftHandWeaponUpdate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEquippedMagicUpdate);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SNDPG_API USNEquipmentComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USNEquipmentComponent();

	UFUNCTION(BlueprintCallable, Category = "SN|EquipmentComponent")
	bool AddToEquippedItems(USNItemBase* ItemToAdd);

	UFUNCTION(BlueprintCallable, Category = "SN|EquipmentComponent")
	bool RemoveFromEquippedItems(USNItemBase* ItemToRemove);
	
	UFUNCTION(BlueprintPure, Category = "SN|EquipmentComponent")
	static USNEquipmentComponent* FindEquipmentComponent(const AActor* Actor)  {return (Actor ? Actor->FindComponentByClass<USNEquipmentComponent>() : nullptr);}

	UFUNCTION()
	bool SwitchEquippedConsumable(int16 CurrentlyEquippedConsumableIndex);

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SN|EquipmentComponent")
	USNItemBase* CurrentlyEquippedConsumable = nullptr;
	
	int16 CurrentlyEquippedConsumableIndex = 0;
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	
protected:
	UPROPERTY(EditDefaultsOnly, Instanced, Category = "SN|EquipmentComponent")
	TArray<class USNItemBase*> DefaultEquippedItems;
	
	UPROPERTY(BlueprintAssignable, Category = "SN|EquipmentComponent")
	FOnEquipmentUpdate OnEquipmentUpdatedDelegate;

	UPROPERTY(BlueprintAssignable, Category = "SN|EquipmentComponent")
	FOnEquippedConsumableUpdate OnEquippedConsumableUpdateDelegate;

	UPROPERTY(BlueprintAssignable, Category = "SN|EquipmentComponent")
	FOnEquippedRightHandWeaponUpdate OnEquippedRightHandWeaponUpdateDelegate;

	UPROPERTY(BlueprintAssignable, Category = "SN|EquipmentComponent")
	FOnEquippedLeftHandWeaponUpdate OnEquippedLeftHandWeaponUpdateDelegate;

	// For the time being unused, but have plans to implement magic in the future
	UPROPERTY(BlueprintAssignable, Category = "SN|EquipmentComponent")
	FOnEquippedMagicUpdate OnEquippedMagicUpdateDelegate;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SN|InventoryComponent|Items")
	TArray<class USNItemBase*> EquippedItems;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SN|InventoryComponent|Items")
	TArray<class USNItemBase*> EquippedConsumables;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SN|InventoryComponent|Items")
	TArray<class USNItemBase*> EquippedLeftHandWeapon;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SN|InventoryComponent|Items")
	TArray<class USNItemBase*> EquippedRightHandWeapon;

	// For the time being unused, but have plans to implement magic in the future
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SN|InventoryComponent|Items")
	TArray<class USNItemBase*> EquippedMagic;
};
