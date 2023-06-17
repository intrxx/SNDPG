// Copyright 2023 Michal Oginski.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SNInventoryComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInventoryUpdated);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SNDPG_API USNInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	USNInventoryComponent();

	bool AddItem(class USNItemBase* ItemToAdd);
	bool RemoveItem(class USNItemBase* ItemToRemove);

	UFUNCTION(BlueprintPure, Category = "Inventory")
	static USNInventoryComponent* FindInventoryComponent(const AActor* Actor) {return (Actor ? Actor->FindComponentByClass<USNInventoryComponent>() : nullptr);}

public:
	UPROPERTY(EditDefaultsOnly, Category = "SN|InventoryComponent")
	int32 InventoryCapacity = 0;
	
protected:
	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditDefaultsOnly, Instanced, Category = "SN|InventoryComponent")
	TArray<class USNItemBase*> DefaultItems;
	
	UPROPERTY(BlueprintAssignable, Category = "SN|InventoryComponent")
	FOnInventoryUpdated OnInventoryUpdatedDelegate;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SN|InventoryComponent|Items")
	TArray<class USNItemBase*> Items;
	
};
