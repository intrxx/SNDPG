// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SNEquipmentSlotButton.generated.h"
/**
 * Enum used to categorise the slot 
 */
UENUM(BlueprintType)
enum class ESlotCategory : uint8
{
	None,
	LeftHandWeaponSlot,
	RightHandWeaponSlot,
	ConsumableSlot,
	ArmourSlot,
	TalismanSlot,
	MagicSlot,
	ArrowSlot,
	BoltSlot
};
/**
 * 
 */
UCLASS()
class SNDPG_API USNEquipmentSlotButton : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI|SlotCategory")
	ESlotCategory SlotCategory;
};
