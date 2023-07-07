// Copyright 2023 Michal Oginski.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SNInventoryComponent.generated.h"

class USNItemBase;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInventoryUpdated);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SNDPG_API USNInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	USNInventoryComponent();

	bool AddItem(USNItemBase* ItemToAdd);
	bool RemoveItem(USNItemBase* ItemToRemove);

	UFUNCTION(BlueprintPure, Category = "SN|InventoryComponent")
	static USNInventoryComponent* FindInventoryComponent(const AActor* Actor) {return (Actor ? Actor->FindComponentByClass<USNInventoryComponent>() : nullptr);}

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
