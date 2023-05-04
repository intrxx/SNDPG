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

protected:
	virtual void BeginPlay() override;
	
	void InputAbilityInputTagPressed(FGameplayTag InputTag);
	void InputAbilityInputTagReleased(FGameplayTag InputTag);

	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SN|Abilities")
	USNAbilitySet* AbilitySet;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SN|Input")
	USNInputConfig* InputConfig;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SN|Input")
	class UInputMappingContext* DefaultMappingContext;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SN|Character", Meta = (AllowPrivateAccess = "true"))
	UCameraComponent* CameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SN|Character", Meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* SpringArmComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SN|Character", Meta = (AllowPrivateAccess = "true"))
	USNBasicAttributesComponent* AttributesComponent;
};
