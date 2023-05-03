// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "GAS/SNAbilitySet.h"
#include "SNCharacterBase.generated.h"

UCLASS()
class SNDPG_API ASNCharacterBase : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASNCharacterBase(const FObjectInitializer& ObjectInitializer);

	// Implementing IAbilitySystemInterface
	virtual class UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	/**
	* Getters for attributes from GAttributeSet
	**/
	UFUNCTION(BlueprintCallable, Category = "SN|Character|Attributes")
	float GetHealth() const;

	UFUNCTION(BlueprintCallable, Category = "SN|Character|Attributes")
	float GetMaxHealth() const;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	TWeakObjectPtr<class USNAbilitySystemComponent> AbilitySystemComponent;
	TWeakObjectPtr<class USNArenaAttributeSet> AttributeSet;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
};
