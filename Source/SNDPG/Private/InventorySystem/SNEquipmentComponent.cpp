// Copyright 2023 Michał Ogiński.


#include "InventorySystem/SNEquipmentComponent.h"
#include "GAS/SNAbilitySet.h"
#include "AbilitySystemGlobals.h"
#include "ActorComponents/SNBasicAttributesComponent.h"
#include "Characters/Hero/SNHero.h"
#include "Characters/Hero/Miscellaneous/SNHeroController.h"
#include "GAS/SNAbilitySystemComponent.h"
#include "InventorySystem/SNItemBase.h"
#include "InventorySystem/SNThrowingKnifeItem.h"
#include "InventorySystem/Items/SNWeaponItem.h"
#include "UI/HeroHUD/SNCharacterStatusWidget.h"

USNEquipmentComponent::USNEquipmentComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	PrimaryComponentTick.bStartWithTickEnabled = false;

	OnEquippedLeftHandWeaponUpdateDelegate.AddDynamic(this, &ThisClass::UpdateWeaponDamageStats);
	OnEquippedRightHandWeaponUpdateDelegate.AddDynamic(this, &ThisClass::UpdateWeaponDamageStats);
	OnEquippedConsumableUpdateDelegate.AddDynamic(this, &ThisClass::UpdateThrowingItemDamageStats);
}

bool USNEquipmentComponent::AddToEquippedItems(USNItemBase* ItemToAdd, ESlotCategory SlotCategory)
{
	if(!ItemToAdd)
	{
		return false;
	}

	ASNHero* HeroOwner = Cast<ASNHero>(GetOwner());
	if(HeroOwner)
	{
		const USNBasicAttributesComponent* AttributesComp = USNBasicAttributesComponent::FindAttributeComponent(HeroOwner);
		if(AttributesComp)
		{
			float CharacterLevel = AttributesComp->GetCharacterLevel();
			if(ItemToAdd->LevelRequirement > CharacterLevel)
			{
				return false;
			}
		}
	}
	
	ItemToAdd->OwningEquipment = this;
	ItemToAdd->World = GetWorld();
	EquippedItems.Add(ItemToAdd);

	switch (SlotCategory)
	{
	case ESlotCategory::LeftHandWeaponSlot:
		EquippedLeftHandWeapon.Add(ItemToAdd);
		
		CurrentlyEquippedLeftHandWeapon = EquippedLeftHandWeapon[0];
		
		OnEquippedLeftHandWeaponUpdateDelegate.Broadcast();
		break;
		
	case ESlotCategory::RightHandWeaponSlot:
		EquippedRightHandWeapon.Add(ItemToAdd);

		RemoveUnequippedItemAbilitySet(CurrentlyEquippedRightHandWeapon);
		CurrentlyEquippedRightHandWeapon = EquippedRightHandWeapon[0];
		AddEquippedItemAbilitySet(CurrentlyEquippedRightHandWeapon);
		
		OnEquippedRightHandWeaponUpdateDelegate.Broadcast();
		break;
		
	case ESlotCategory::ConsumableSlot:
		EquippedConsumables.Add(ItemToAdd);
		
		RemoveUnequippedItemAbilitySet(CurrentlyEquippedConsumable);
		CurrentlyEquippedConsumable = EquippedConsumables[0];
		AddEquippedItemAbilitySet(CurrentlyEquippedConsumable);
		
		OnEquippedConsumableUpdateDelegate.Broadcast();
		break;

	case ESlotCategory::MagicSlot:
		EquippedMagic.Add(ItemToAdd);

		RemoveUnequippedItemAbilitySet(CurrentlyEquippedMagic);
		CurrentlyEquippedMagic = EquippedMagic[0];
		AddEquippedItemAbilitySet(CurrentlyEquippedMagic);

		OnEquippedMagicUpdateDelegate.Broadcast();
		break;
		
	case ESlotCategory::ArmourSlot:
		AddEquippedItemAbilitySet(ItemToAdd);
		break;

	case ESlotCategory::TalismanSlot:
		AddEquippedItemAbilitySet(ItemToAdd);
		break;
	
	default:
		OnEquipmentUpdatedDelegate.Broadcast();
		break;
	}
	
	return true;
}

bool USNEquipmentComponent::RemoveFromEquippedItems(USNItemBase* ItemToRemove, ESlotCategory SlotCategory)
{
	if(!ItemToRemove)
	{
		return false;
	}

	ItemToRemove->OwningEquipment = nullptr;
	ItemToRemove->World = nullptr;
	EquippedItems.RemoveSingle(ItemToRemove);
	RemoveUnequippedItemAbilitySet(ItemToRemove);
	
	switch (SlotCategory)
	{
	case ESlotCategory::LeftHandWeaponSlot:
		EquippedLeftHandWeapon.RemoveSingle(ItemToRemove);
		
		if (!EquippedLeftHandWeapon.IsEmpty())
		{
			CurrentlyEquippedLeftHandWeapon = EquippedLeftHandWeapon[0];
		}
		else
		{
			CurrentlyEquippedLeftHandWeapon = nullptr;
		}
		
		OnEquippedLeftHandWeaponUpdateDelegate.Broadcast();
		break;
		
	case ESlotCategory::RightHandWeaponSlot:
		EquippedRightHandWeapon.RemoveSingle(ItemToRemove);
		
		if (!EquippedRightHandWeapon.IsEmpty())
		{
			CurrentlyEquippedRightHandWeapon = EquippedRightHandWeapon[0];
			AddEquippedItemAbilitySet(CurrentlyEquippedRightHandWeapon);
		}
		else
		{
			CurrentlyEquippedRightHandWeapon = nullptr;
		}

		OnEquippedRightHandWeaponUpdateDelegate.Broadcast();
		break;
		
	case ESlotCategory::ConsumableSlot:
		EquippedConsumables.RemoveSingle(ItemToRemove);
		UE_LOG(LogTemp, Warning, TEXT("Removing item from consumable slot: %s"), *ItemToRemove->GetName());
		
		if (!EquippedConsumables.IsEmpty())
		{
			CurrentlyEquippedConsumable = EquippedConsumables[0];
			AddEquippedItemAbilitySet(CurrentlyEquippedConsumable);
		}
		else
		{
			CurrentlyEquippedConsumable = nullptr;
		}
		
		OnEquippedConsumableUpdateDelegate.Broadcast();
		break;

	case ESlotCategory::MagicSlot:
		EquippedMagic.RemoveSingle(ItemToRemove);

		if (!EquippedConsumables.IsEmpty())
		{
			CurrentlyEquippedMagic = EquippedMagic[0];
			AddEquippedItemAbilitySet(CurrentlyEquippedMagic);
		}
		else
		{
			CurrentlyEquippedMagic = nullptr;
		}
		
		OnEquippedMagicUpdateDelegate.Broadcast();
		break;
		
	case ESlotCategory::ArmourSlot:
		RemoveUnequippedItemAbilitySet(ItemToRemove);
		break;

	case ESlotCategory::TalismanSlot:
		RemoveUnequippedItemAbilitySet(ItemToRemove);
		break;
	
	default:
		OnEquipmentUpdatedDelegate.Broadcast();
	}
	
	return true;
}

bool USNEquipmentComponent::SwitchEquippedConsumable(int16 Index)
{
	if(EquippedConsumables.IsEmpty() || EquippedConsumables.Num() == 1)
	{
		return false;
	}
	
	if(Index+1 < EquippedConsumables.Num())
	{
		RemoveUnequippedItemAbilitySet(CurrentlyEquippedConsumable);
		
		CurrentlyEquippedConsumable = EquippedConsumables[Index+1];
		CurrentlyEquippedConsumableIndex = Index+1;

		AddEquippedItemAbilitySet(CurrentlyEquippedConsumable);

		OnEquippedConsumableUpdateDelegate.Broadcast();
		UE_LOG(LogTemp, Warning, TEXT("Curentely Equipped Consumable: %s"), *CurrentlyEquippedConsumable->GetName());
		return true;
	}

	if(Index+1 == EquippedConsumables.Num())
	{
		RemoveUnequippedItemAbilitySet(CurrentlyEquippedConsumable);
		
		CurrentlyEquippedConsumable = EquippedConsumables[0];
		CurrentlyEquippedConsumableIndex = 0;

		AddEquippedItemAbilitySet(CurrentlyEquippedConsumable);

		OnEquippedConsumableUpdateDelegate.Broadcast();
		UE_LOG(LogTemp, Warning, TEXT("Curentely Equipped Consumable: %s"), *CurrentlyEquippedConsumable->GetName());
		return true;
	}
	
	return false;
}

bool USNEquipmentComponent::SwitchEquippedLeftHandWeapon(int16 Index)
{
	if(EquippedLeftHandWeapon.IsEmpty() || EquippedLeftHandWeapon.Num() == 1)
	{
		return false;
	}
	
	if(Index+1 < EquippedLeftHandWeapon.Num())
	{
		CurrentlyEquippedLeftHandWeapon = EquippedLeftHandWeapon[Index+1];
		CurrentlyEquippedLeftHandWeaponIndex = Index+1;
		
		OnEquippedLeftHandWeaponUpdateDelegate.Broadcast();
		UE_LOG(LogTemp, Warning, TEXT("Curentely Equipped Left Hand Weapon: %s"), *CurrentlyEquippedLeftHandWeapon->GetName());
		return true;
	}

	if(Index+1 == EquippedLeftHandWeapon.Num())
	{
		CurrentlyEquippedLeftHandWeapon = EquippedLeftHandWeapon[0];
		CurrentlyEquippedLeftHandWeaponIndex = 0;
		
		OnEquippedLeftHandWeaponUpdateDelegate.Broadcast();
		UE_LOG(LogTemp, Warning, TEXT("Curentely Equipped Left Hand Weapon: %s"), *CurrentlyEquippedLeftHandWeapon->GetName());
		return true;
	}
	
	return false;
}

bool USNEquipmentComponent::SwitchEquippedRightHandWeapon(int16 Index)
{
	if(EquippedRightHandWeapon.IsEmpty() || EquippedRightHandWeapon.Num() == 1)
	{
		return false;
	}
	
	if(Index+1 < EquippedRightHandWeapon.Num())
	{
		RemoveUnequippedItemAbilitySet(CurrentlyEquippedRightHandWeapon);
		
		CurrentlyEquippedRightHandWeapon = EquippedRightHandWeapon[Index+1];
		CurrentlyEquippedRightHandWeaponIndex = Index+1;

		AddEquippedItemAbilitySet(CurrentlyEquippedRightHandWeapon);

		OnEquippedRightHandWeaponUpdateDelegate.Broadcast();
		UE_LOG(LogTemp, Warning, TEXT("Curentely Equipped Right Hand Weapon: %s"), *CurrentlyEquippedRightHandWeapon->GetName());
		return true;
	}

	if(Index+1 == EquippedRightHandWeapon.Num())
	{
		RemoveUnequippedItemAbilitySet(CurrentlyEquippedRightHandWeapon);
		
		CurrentlyEquippedRightHandWeapon = EquippedRightHandWeapon[0];
		CurrentlyEquippedRightHandWeaponIndex = 0;

		AddEquippedItemAbilitySet(CurrentlyEquippedRightHandWeapon);

		OnEquippedRightHandWeaponUpdateDelegate.Broadcast();
		UE_LOG(LogTemp, Warning, TEXT("Curentely Equipped Right Hand Weapon: %s"), *CurrentlyEquippedRightHandWeapon->GetName());
		return true;
	}
	
	return false;
}

bool USNEquipmentComponent::SwitchEquippedMagic(int16 Index)
{
	if(EquippedMagic.IsEmpty() || EquippedMagic.Num() == 1)
	{
		return false;
	}
	
	if(Index+1 < EquippedMagic.Num())
	{
		RemoveUnequippedItemAbilitySet(CurrentlyEquippedMagic);
		
		CurrentlyEquippedMagic = EquippedMagic[Index+1];
		CurrentlyEquippedMagicIndex = Index+1;

		AddEquippedItemAbilitySet(CurrentlyEquippedMagic);
		
		UE_LOG(LogTemp, Warning, TEXT("Curentely Equipped Magic: %s"), *CurrentlyEquippedMagic->GetName());
		return true;
	}
                                                                                                                   
	if(Index+1 == EquippedMagic.Num())
	{
		RemoveUnequippedItemAbilitySet(CurrentlyEquippedMagic);
		
		CurrentlyEquippedMagic = EquippedMagic[0];
		CurrentlyEquippedMagicIndex = 0;

		AddEquippedItemAbilitySet(CurrentlyEquippedMagic);
		
		UE_LOG(LogTemp, Warning, TEXT("Curentely Equipped Magic: %s"), *CurrentlyEquippedMagic->GetName());
		return true;
	}
	
	return false;
}

bool USNEquipmentComponent::IsUnarmed() const
{
	if(!CurrentlyEquippedLeftHandWeapon && !CurrentlyEquippedRightHandWeapon)
	{
		return true;
	}
	return false;
}

float USNEquipmentComponent::GetEquippedLightAttackWeaponDamage() const
{
	float Damage = 0.0f;

	if(IsUnarmed())
	{
		return Damage = 10.0f;
	}
	
	if(USNWeaponItem* RightHandWeapon = Cast<USNWeaponItem>(CurrentlyEquippedRightHandWeapon))
	{
		Damage += RightHandWeapon->LightAttackDamage;
	}
	
	if(USNWeaponItem* LeftHandWeapon = Cast<USNWeaponItem>(CurrentlyEquippedLeftHandWeapon))
	{
		Damage +=LeftHandWeapon->LightAttackDamage;
	}
	
	return Damage;
}

float USNEquipmentComponent::GetEquippedHeavyAttackWeaponDamage() const
{
	float Damage = 0.0f;

	if(IsUnarmed())
	{
		return Damage = 20.0f;
	}
	
	if(USNWeaponItem* RightHandWeapon = Cast<USNWeaponItem>(CurrentlyEquippedRightHandWeapon))
	{
		Damage += RightHandWeapon->HeavyAttackDamage;
	}
	
	if(USNWeaponItem* LeftHandWeapon = Cast<USNWeaponItem>(CurrentlyEquippedLeftHandWeapon))
	{
		Damage +=LeftHandWeapon->HeavyAttackDamage;
	}
	
	return Damage;
}

float USNEquipmentComponent::GetEquippedWeaponSpellDamage() const
{
	float Damage = 0.0f;

	if(IsUnarmed())
	{
		return Damage = 0.0f;
	}
	
	if(USNWeaponItem* RightHandWeapon = Cast<USNWeaponItem>(CurrentlyEquippedRightHandWeapon))
	{
		Damage += RightHandWeapon->WeaponSpellDamage;
	}

	return Damage;
}

float USNEquipmentComponent::GetEquippedThrowingWeaponDamage() const
{
	float Damage = 0.0f;

	if(USNThrowingKnifeItem* ThrowingWeaponItem = Cast<USNThrowingKnifeItem>(CurrentlyEquippedConsumable))
	{
		Damage += ThrowingWeaponItem->ThrowingWeaponDamage;
	}

	return Damage;
}

void USNEquipmentComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

USNAbilitySystemComponent* USNEquipmentComponent::GetAbilitySystemComponent() const
{
	AActor* OwningActor = GetOwner();
	return Cast<USNAbilitySystemComponent>(UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(OwningActor));
}

void USNEquipmentComponent::AddEquippedItemAbilitySet(USNItemBase* Item)
{
	if(USNAbilitySystemComponent* ASC = GetAbilitySystemComponent())
	{
		if(Item->AbilitySet)
		{
			Item->AbilitySet->GiveToAbilitySystem(ASC, &Item->GrantedHandles, Item);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("AbilitySet on the item is not valid, check if it is added to the item"));
		}
	}
}

void USNEquipmentComponent::RemoveUnequippedItemAbilitySet(USNItemBase* Item)
{
	if(!Item)
	{
		return;
	}
	
	if(USNAbilitySystemComponent* ASC = GetAbilitySystemComponent())
	{
		Item->GrantedHandles.TakeFromAbilitySystem(ASC);
	}
}

void USNEquipmentComponent::UpdateWeaponDamageStats()
{
	ASNHero* Hero = Cast<ASNHero>(GetOwner());
	if (Hero)
	{
		ASNHeroController* PC = Cast<ASNHeroController>(Hero->GetController());
		if(PC)
		{
			USNCharacterStatusWidget* Status = Cast<USNCharacterStatusWidget>(PC->GetCharacterStatusUI());
			if(Status)
			{
				const USNBasicAttributesComponent* Attributes = USNBasicAttributesComponent::FindAttributeComponent(Hero);
				check(Attributes);
				
				Status->SetLightAttackRange(GetEquippedLightAttackWeaponDamage(), Attributes->GetStrength());
				Status->SetHeavyAttackRange(GetEquippedHeavyAttackWeaponDamage(), Attributes->GetStrength());
				Status->SetWeaponSpellDamage(GetEquippedWeaponSpellDamage(), Attributes->GetArcane());
			}
		}
	}
}

void USNEquipmentComponent::UpdateThrowingItemDamageStats()
{
	ASNHero* Hero = Cast<ASNHero>(GetOwner());
	if (Hero)
	{
		ASNHeroController* PC = Cast<ASNHeroController>(Hero->GetController());
		if(PC)
		{
			USNCharacterStatusWidget* Status = Cast<USNCharacterStatusWidget>(PC->GetCharacterStatusUI());
			if(Status)
			{
				const USNBasicAttributesComponent* Attributes = USNBasicAttributesComponent::FindAttributeComponent(Hero);
				check(Attributes);
				
				Status->SetItemDamageRange(GetEquippedThrowingWeaponDamage(), Attributes->GetStrength());
			}
		}
	}
}


