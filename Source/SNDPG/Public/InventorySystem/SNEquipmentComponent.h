// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SNEquipmentComponent.generated.h"

enum class EItemCategory : uint8;
class USNItemBase;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEquipmentUpdate);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SNDPG_API USNEquipmentComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USNEquipmentComponent();

	UFUNCTION(BlueprintCallable, Category = "SN|EquipmentComponent")
	bool AddToEquippedItems(USNItemBase* ItemToAdd, EItemCategory ItemCategory);

	UFUNCTION(BlueprintCallable, Category = "SN|EquipmentComponent")
	bool RemoveFromEquippedItems(USNItemBase* ItemToRemove, EItemCategory ItemCategory);
	
	UFUNCTION(BlueprintCallable, Category = "SN|EquipmentComponent")
	USNEquipmentComponent* FindEquipment(const AActor* Actor)  {return (Actor ? Actor->FindComponentByClass<USNEquipmentComponent>() : nullptr); }
protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	
protected:
	UPROPERTY(EditDefaultsOnly, Instanced, Category = "SN|EquipmentComponent")
	TArray<class USNItemBase*> DefaultEquippedItems;
	
	UPROPERTY(BlueprintAssignable, Category = "SN|EquipmentComponent")
	FOnEquipmentUpdate OnEquipmentUpdatedDelegate;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SN|InventoryComponent|Items")
	TArray<class USNItemBase*> EquippedItems;
		
};
