// Copyright 2023 Michal Oginski.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GAS/SNAbilitySystemComponent.h"
#include "SNBasicAttributesComponent.generated.h"

class USNAbilitySystemComponent;
class USNBasicAttributes;
/**
 * ESNDeathState
 *
 *	Defines current state of death.
 */
UENUM(BlueprintType)
enum class ESNDeathState : uint8
{
	NotDead = 0,
	DeathStarted,
	DeathFinished
};

/**
 * USNBasicAttributesComponent
 *
 *	An actor component used to handle anything related to basic attributes
 */
UCLASS( Blueprintable, meta=(BlueprintSpawnableComponent) )
class SNDPG_API USNBasicAttributesComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USNBasicAttributesComponent();
	
	// Returns the basic attributes component if one exists on the specified actor.
	UFUNCTION(BlueprintPure, Category = "NS|Attributes")
	static USNBasicAttributesComponent* FindAttributeComponent(const AActor* Actor) { return (Actor ? Actor->FindComponentByClass<USNBasicAttributesComponent>() : nullptr); }

	// Initialize the component using an ability system component.
	UFUNCTION(BlueprintCallable, Category = "Lyra|Attributes")
	void InitializeWithAbilitySystem(USNAbilitySystemComponent* InASC);

	// Uninitialize the component, clearing any references to the ability system.
	UFUNCTION(BlueprintCallable, Category = "Lyra|Attributes")
	void UninitializeFromAbilitySystem();
	
	// Returns the current health value.
	UFUNCTION(BlueprintCallable, Category = "Lyra|Attributes")
	float GetHealth() const;
	
	// Returns the current maximum health value.
	UFUNCTION(BlueprintCallable, Category = "Lyra|Attributes")
	float GetMaxHealth() const;
	
	UFUNCTION(BlueprintCallable, Category = "Lyra|Attributes")
	FGameplayAttribute GetHealthAttributes() const;
	
	UFUNCTION(BlueprintCallable, Category = "Lyra|Attributes")
	float GetResource() const;
	
	// Returns the current maximum health value.
	UFUNCTION(BlueprintCallable, Category = "Lyra|Attributes")
	float GetMaxResource() const;

	UFUNCTION(BlueprintCallable, Category = "Lyra|Attributes")
	ESNDeathState GetDeathState() const { return DeathState; }
	
	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "Lyra|Attributes", Meta = (ExpandBoolAsExecs = "ReturnValue"))
	bool IsDeadOrDying() const {return (DeathState > ESNDeathState::NotDead);}
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	virtual void HealthChanged(const FOnAttributeChangeData& Data);
	virtual void MaxHealthChanged(const FOnAttributeChangeData& Data);
	virtual void ResourceChanged(const FOnAttributeChangeData& Data);
	virtual void MaxResourceChanged(const FOnAttributeChangeData& Data);
	
	
protected:
	FDelegateHandle HealthChangedDelegateHandle;
	FDelegateHandle MaxHealthChangedDelegateHandle;
	FDelegateHandle ResourceChangedDelegateHandle;
	FDelegateHandle MaxResourceChangedDelegateHandle;
	
	UPROPERTY()
	USNAbilitySystemComponent* AbilitySystemComponent;

	UPROPERTY()
	const USNBasicAttributes* BasicAttributes;
	
	UPROPERTY()
	ESNDeathState DeathState;

public:	
	

		
};
