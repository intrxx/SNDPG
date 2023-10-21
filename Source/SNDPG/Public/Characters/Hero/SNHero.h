// Copyright 2023 Michal Oginski.

#pragma once

#include "CoreMinimal.h"
#include "Characters/SNCharacterBase.h"
#include "GameplayTagContainer.h"
#include "InputActionValue.h"
#include "GAS/SNGameplayAbility.h"
#include "SNHero.generated.h"

class USNEquipmentComponent;
class USNInventoryComponent;
class USNCombatComponent;
class USNInputConfig;
class UInputMappingContext;
class UCameraComponent;
class USNAbilitySet;
class USpringArmComponent;
class USNBasicAttributesComponent;

namespace Hero
{
	static const float LookYawRate = 245.0f;
	static const float LookPitchRate = 145.0f;
}

/**
 *  ASNHero
 *
 *  Base class for all hero type characters in this project
 */
UCLASS()
class SNDPG_API ASNHero : public ASNCharacterBase
{
	GENERATED_BODY()

public:
	ASNHero(const FObjectInitializer& ObjectInitializer);
	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void PossessedBy(AController* NewController) override;
	
	UFUNCTION()
	virtual void OnDeathStarted(AActor* OwningActor);
	
	UFUNCTION()
	virtual void OnDeathFinished(AActor* OwningActor);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void RemoveStaminaBlockTag(float Count);

	UFUNCTION(BlueprintCallable, Category = "SN|InventorySystem")
	void UseItem(class USNItemBase* ItemToUse);

	UFUNCTION(BlueprintCallable, Category = "SN|InputConfig")
	void ApplyGameplayInputMappings();

protected:
	virtual void BeginPlay() override;

	virtual void DestroyDueToDeath() override;
	
	void InputAbilityInputTagPressed(FGameplayTag InputTag);
	void InputAbilityInputTagReleased(FGameplayTag InputTag);

	void Move(const FInputActionValue& Value);
	void LookMouse(const FInputActionValue& Value);
	void LookStick(const FInputActionValue& Value);

	void SwitchItemRight_Weapon();
	void SwitchItemLeft_Weapon();
	void SwitchItemDown_Consumable();
	void SwitchItemUp_Magic();
	
	void ToggleInGameMenu();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SN|Abilities")
	TObjectPtr<USNAbilitySet> AbilitySet;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SN|Input")
	TObjectPtr<USNInputConfig> InputConfig;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SN|Input")
	TObjectPtr<UInputMappingContext> DefaultMappingContext_MNK;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SN|Input")
	TObjectPtr<UInputMappingContext> DefaultMappingContext_Gamepad;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SN|Input")
	TObjectPtr<UInputMappingContext> HUDMappingContext;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SN|Character", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCameraComponent> CameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SN|Character", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USpringArmComponent> SpringArmComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SN|Character", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USNBasicAttributesComponent> AttributesComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SN|Character", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USNCombatComponent> CombatComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SN|Character", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USNInventoryComponent> InventoryComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SN|Character", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USNEquipmentComponent> EquipmentComponent;
};
