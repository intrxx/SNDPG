// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/SNCharacterBase.h"
#include "Characters/SNCharacterMovementComponent.h"
#include "GAS/Attributes/SNArenaAttributeSet.h"
#include "GAS/SNAbilitySystemComponent.h"

// Sets default values
ASNCharacterBase::ASNCharacterBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<USNCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

UAbilitySystemComponent* ASNCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent.Get();
}

float ASNCharacterBase::GetHealth() const
{
	if(AttributeSet.IsValid())
	{
		return AttributeSet->GetHealth();
	}
	return 0.f;
}

float ASNCharacterBase::GetMaxHealth() const
{
	if(AttributeSet.IsValid())
	{
		return AttributeSet->GetMaxHealth();
	}
	return 0.f;
}

// Called when the game starts or when spawned
void ASNCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASNCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ASNCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

