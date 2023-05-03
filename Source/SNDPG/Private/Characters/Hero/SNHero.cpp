// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Hero/SNHero.h"
#include "GAS/SNAbilitySystemComponent.h"
#include "Characters/SNCharacterMovementComponent.h"
#include "GameplayTags/SNGameplayTags.h"
#include "Input/SNEnhancedInputComponent.h"

ASNHero::ASNHero(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void ASNHero::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	USNEnhancedInputComponent* EInputComponent = Cast<USNEnhancedInputComponent>(PlayerInputComponent);
	check(EInputComponent);

	const FSNGameplayTags& GameplayTags = FSNGameplayTags::Get();
	TArray<uint32> BindHandles;
	EInputComponent->BindAbilityActions(InputConfig, this, &ThisClass::InputAbilityInputTagPressed,
		&ThisClass::InputAbilityInputTagReleased, /*out*/ BindHandles);
}

void ASNHero::InputAbilityInputTagPressed(FGameplayTag InputTag)
{
	AbilitySystemComponent->AbilityInputTagPressed(InputTag);
}

void ASNHero::InputAbilityInputTagReleased(FGameplayTag InputTag)
{
	AbilitySystemComponent->AbilityInputTagReleased(InputTag);
}

void ASNHero::AddCharacterAttributes()
{
}

void ASNHero::AddCharacterAbilitiesAndEffects()
{
}


