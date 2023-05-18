// Copyright 2023 Michal Oginski.


#include "Characters/SNCharacterBase.h"
#include "Characters/SNCharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "GAS/SNAbilitySystemComponent.h"

// Sets default values
ASNCharacterBase::ASNCharacterBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<USNCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

}

UAbilitySystemComponent* ASNCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent.Get();
}

void ASNCharacterBase::OnDeathStarted(AActor* OwningActor)
{
	DisableMovementAndCollision();
}

void ASNCharacterBase::OnDeathFinished(AActor* OwningActor)
{
	GetWorld()->GetTimerManager().SetTimerForNextTick(this, &ThisClass::DestroyDueToDeath);
}

/*
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
*/
// Called when the game starts or when spawned
void ASNCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASNCharacterBase::DisableMovementAndCollision()
{
	if(Controller)
	{
		Controller->SetIgnoreMoveInput(true);
		Controller->SetIgnoreLookInput(true);
	}
	
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECR_Ignore);

	USNCharacterMovementComponent* SNMoveComp = Cast<USNCharacterMovementComponent>(GetCharacterMovement());
	SNMoveComp->StopMovementImmediately();
	SNMoveComp->DisableMovement();
}

void ASNCharacterBase::DestroyDueToDeath()
{
	K2_OnDeathFinished();

	UninitAndDestroy();
}

void ASNCharacterBase::UninitAndDestroy()
{
	if (GetLocalRole() == ROLE_Authority)
	{
		DetachFromControllerPendingDestroy();
		SetLifeSpan(0.1f);
	}

	
	if (USNAbilitySystemComponent* SNASC = Cast<USNAbilitySystemComponent>(GetAbilitySystemComponent()))
	{
	
		if (SNASC->GetAvatarActor() == this)
		{
			UninitializeAbilitySystem();
		}
	}

	SetActorHiddenInGame(true);
}

void ASNCharacterBase::UninitializeAbilitySystem()
{
	if (!AbilitySystemComponent.IsValid())
	{
		return;
	}
	
	if (AbilitySystemComponent->GetAvatarActor() == GetOwner())
	{
		AbilitySystemComponent->CancelAbilities(nullptr, nullptr);
		AbilitySystemComponent->ClearAbilityInput();
		AbilitySystemComponent->RemoveAllGameplayCues();

		if (AbilitySystemComponent->GetOwnerActor() != nullptr)
		{
			AbilitySystemComponent->SetAvatarActor(nullptr);
		}
		else
		{
			// If the ASC doesn't have a valid owner, we need to clear *all* actor info, not just the avatar pairing
			AbilitySystemComponent->ClearActorInfo();
		}
	}

	AbilitySystemComponent = nullptr;
}



