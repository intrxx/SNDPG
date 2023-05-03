// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "Characters/SNCharacterBase.h"
#include "Input/SNInputConfig.h"
#include "InputActionValue.h"
#include "GameFramework/SpringArmComponent.h"
#include "SNHero.generated.h"

/**
 * 
 */
UCLASS()
class SNDPG_API ASNHero : public ASNCharacterBase
{
	GENERATED_BODY()

public:
	ASNHero(const FObjectInitializer& ObjectInitializer);

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	virtual void BeginPlay() override;
	
	void AddCharacterAttributes();
	void AddCharacterAbilitiesAndEffects();

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
	
	FSNAbilitySet_GrantedHandles* GrantedHandles;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SN|Character", Meta = (AllowPrivateAccess = "true"))
	UCameraComponent* CameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SN|Character", Meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* SpringArmComp;
};
