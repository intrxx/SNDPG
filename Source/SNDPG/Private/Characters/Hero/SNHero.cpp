// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Hero/SNHero.h"
#include "GAS/SNAbilitySystemComponent.h"
#include "Characters/SNCharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameplayTags/SNGameplayTags.h"
#include "Input/SNEnhancedInputComponent.h"

ASNHero::ASNHero(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	UCapsuleComponent* CapsuleComp = GetCapsuleComponent();
	check(CapsuleComp);
	CapsuleComp->InitCapsuleSize(44.0f, 150.0f);

	USkeletalMeshComponent* MeshComp = GetMesh();
	check(MeshComp);
	MeshComp->SetRelativeRotation(FRotator(0.0f, -90.0f,0.0f));
	MeshComp->SetRelativeLocation(FVector(0.0f,0.0f,-110.0f));

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArmComp->SetupAttachment(RootComponent);
	SpringArmComp->SetRelativeLocation(FVector(0.0f,0.0f,65.0f));
	SpringArmComp->TargetArmLength = 500;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(SpringArmComp);
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


