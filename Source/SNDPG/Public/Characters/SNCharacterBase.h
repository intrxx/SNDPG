// Copyright 2023 Michal Oginski.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
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

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called when the death sequence for the character has completed
	UFUNCTION(BlueprintImplementableEvent, meta=(DisplayName="OnDeathFinished"))
	void K2_OnDeathFinished();
	
	void DisableMovementAndCollision();
	void UninitAndDestroy();
	void UninitializeAbilitySystem();
	virtual void DestroyDueToDeath();
	
protected:

	TWeakObjectPtr<class USNAbilitySystemComponent> AbilitySystemComponent;
	TWeakObjectPtr<class USNBasicAttributes> AttributeSetBase;
};
