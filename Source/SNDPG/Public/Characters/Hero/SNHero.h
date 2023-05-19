// Copyright 2023 Michal Oginski.

#pragma once

#include "CoreMinimal.h"
#include "Characters/SNCharacterBase.h"
#include "GameplayTagContainer.h"
#include "InputActionValue.h"
#include "SNHero.generated.h"

class USNInputConfig;
class UCameraComponent;
class USNAbilitySet;
class USpringArmComponent;
class USNBasicAttributesComponent;

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

protected:
	virtual void BeginPlay() override;

	virtual void DestroyDueToDeath() override;
	
	void InputAbilityInputTagPressed(FGameplayTag InputTag);
	void InputAbilityInputTagReleased(FGameplayTag InputTag);

	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void ToggleCharacterStatus();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SN|Abilities")
	USNAbilitySet* AbilitySet;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SN|Input")
	USNInputConfig* InputConfig;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SN|Input")
	class UInputMappingContext* DefaultMappingContext;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "SN|Attributes")
	TSubclassOf<class UGameplayEffect> DefaultAttributeEffect;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SN|Character", Meta = (AllowPrivateAccess = "true"))
	UCameraComponent* CameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SN|Character", Meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* SpringArmComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SN|Character", Meta = (AllowPrivateAccess = "true"))
	USNBasicAttributesComponent* AttributesComponent;
};
