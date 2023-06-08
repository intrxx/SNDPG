// Copyright 2023 Michal Oginski.

#pragma once

#include "CoreMinimal.h"
#include "Characters/SNCharacterBase.h"
#include "GameplayTagContainer.h"
#include "InputActionValue.h"
#include "GAS/SNGameplayAbility.h"
#include "SNHero.generated.h"

class USNCombatComponent;
class USNInputConfig;
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

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void PossessedBy(AController* NewController) override;

	// Begins the death sequence for the character (disables collision, disables movement, etc...)
	UFUNCTION()
	virtual void OnDeathStarted(AActor* OwningActor);

	// Ends the death sequence for the character (detaches controller, destroys pawn, etc...)
	UFUNCTION()
	virtual void OnDeathFinished(AActor* OwningActor);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void RemoveStaminaBlockTag(float Count);

public:
	/*
	 * Just a place holder really, in the future the damage should come from items.
	 * TODO Change it to the Weapon Damage in HeroController and BasicAttributesComponent.
	 */
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SN|AbilitiesBaseDamage")
	float R1BaseDamage = 50.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SN|AbilitiesBaseDamage")
	float R2BaseDamage = 100.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SN|AbilitiesBaseDamage")
	float L1BaseDamage = 30.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SN|AbilitiesBaseDamage")
	float WeaponSpellDamage = 200.0f;

	/*
	 */
	
protected:
	virtual void BeginPlay() override;

	virtual void DestroyDueToDeath() override;
	
	void InputAbilityInputTagPressed(FGameplayTag InputTag);
	void InputAbilityInputTagReleased(FGameplayTag InputTag);

	void Move(const FInputActionValue& Value);
	void LookMouse(const FInputActionValue& Value);
	void LookStick(const FInputActionValue& Value);
	
	void ToggleCharacterStatus();

	void SetGamePause(bool bIsPaused);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SN|Abilities")
	USNAbilitySet* AbilitySet;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SN|Input")
	USNInputConfig* InputConfig;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SN|Input")
	class UInputMappingContext* DefaultMappingContext_MNK;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SN|Input")
	class UInputMappingContext* DefaultMappingContext_Gamepad;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SN|Input")
	class UInputMappingContext* HUDMappingContext;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "SN|Attributes")
	TSubclassOf<class UGameplayEffect> DefaultAttributeEffect;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SN|Character", Meta = (AllowPrivateAccess = "true"))
	UCameraComponent* CameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SN|Character", Meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* SpringArmComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SN|Character", Meta = (AllowPrivateAccess = "true"))
	USNBasicAttributesComponent* AttributesComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SN|Character", Meta = (AllowPrivateAccess = "true"))
	USNCombatComponent* CombatComponent;
};
