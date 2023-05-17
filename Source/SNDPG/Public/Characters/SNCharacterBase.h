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

	// Begins the death sequence for the character (disables collision, disables movement, etc...)
	UFUNCTION()
	virtual void OnDeathStarted(AActor* OwningActor);

	// Ends the death sequence for the character (detaches controller, destroys pawn, etc...)
	UFUNCTION()
	virtual void OnDeathFinished(AActor* OwningActor);

	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called when the death sequence for the character has completed
	UFUNCTION(BlueprintImplementableEvent, meta=(DisplayName="OnDeathFinished"))
	void K2_OnDeathFinished();
	
	void DisableMovementAndCollision();
	void DestroyDueToDeath();
	void UninitAndDestroy();

	void UninitializeAbilitySystem();
protected:

	TWeakObjectPtr<class USNAbilitySystemComponent> AbilitySystemComponent;
	TWeakObjectPtr<class USNBasicAttributes> AttributeSetBase;
};
