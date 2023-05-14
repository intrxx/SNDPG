// Copyright 2023 Michal Oginski.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GAS/SNAbilitySystemComponent.h"
#include "GAS/Attributes/SNBasicAttributes.h"
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
	UFUNCTION(BlueprintPure, Category = "SN|Attributes")
	static USNBasicAttributesComponent* FindAttributeComponent(const AActor* Actor) { return (Actor ? Actor->FindComponentByClass<USNBasicAttributesComponent>() : nullptr); }

	// Initialize the component using an ability system component.
	UFUNCTION(BlueprintCallable, Category = "SN|Attributes")
	void InitializeWithAbilitySystem(USNAbilitySystemComponent* InASC);

	// Uninitialize the component, clearing any references to the ability system.
	UFUNCTION(BlueprintCallable, Category = "SN|Attributes")
	void UninitializeFromAbilitySystem();

	UFUNCTION(BlueprintCallable, Category = "SN|Attributes")
	ESNDeathState GetDeathState() const { return DeathState; }
	
	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "Lyra|Attributes", Meta = (ExpandBoolAsExecs = "ReturnValue"))
	bool IsDeadOrDying() const {return (DeathState > ESNDeathState::NotDead);}

	/**
	 * Getters from Attributes
	 */
	
	UFUNCTION(BlueprintCallable, Category = "SN|Attributes")
	float GetHealth() const;
	
	UFUNCTION(BlueprintCallable, Category = "SN|Attributes")
	float GetMaxHealth() const;
	
	UFUNCTION(BlueprintCallable, Category = "SN|Attributes")
	FGameplayAttribute GetHealthAttribute() const {return BasicAttributes->GetHealthAttribute();}

	UFUNCTION(BlueprintCallable, Category = "SN|Attributes")
	FGameplayAttribute GetMaxHealthAttribute() const {return BasicAttributes->GetMaxHealthAttribute();}
	
	UFUNCTION(BlueprintCallable, Category = "SN|Attributes")
	float GetResource() const;
	
	UFUNCTION(BlueprintCallable, Category = "SN|Attributes")
	float GetMaxResource() const;

	UFUNCTION(BlueprintCallable, Category = "SN|Attributes")
	FGameplayAttribute GetResourceAttribute() const {return BasicAttributes->GetResourceAttribute();}
	
	UFUNCTION(BlueprintCallable, Category = "SN|Attributes")
	FGameplayAttribute GetMaxResourceAttribute() const {return BasicAttributes->GetMaxResourceAttribute();}

	UFUNCTION(BlueprintCallable, Category = "SN|Attributes")
	float GetCharacterLevel() const;

	UFUNCTION(BlueprintCallable, Category = "SN|Attributes")
	FGameplayAttribute GetCharacterLevelAttribute() const {return BasicAttributes->GetCharacterLevelAttribute();}
	
	UFUNCTION(BlueprintCallable, Category = "SN|Attributes")
	float GetExperience() const;
	
	UFUNCTION(BlueprintCallable, Category = "SN|Attributes")
	float GetMaxExperience() const;

	UFUNCTION(BlueprintCallable, Category = "SN|Attributes")
	FGameplayAttribute GetExperienceAttribute() const {return BasicAttributes->GetExperienceAttribute();}

	UFUNCTION(BlueprintCallable, Category = "SN|Attributes")
	FGameplayAttribute GetMaxExperienceAttribute() const {return BasicAttributes->GetMaxExperienceAttribute();}

	UFUNCTION(BlueprintCallable, Category = "SN|Attributes")
	float GetArmour() const;

	UFUNCTION(BlueprintCallable, Category = "SN|Attributes")
	FGameplayAttribute GetArmourAttribute() const {return BasicAttributes->GetArmourAttribute();}

	UFUNCTION(BlueprintCallable, Category = "SN|Attributes")
	float GetStrength() const;

	UFUNCTION(BlueprintCallable, Category = "SN|Attributes")
	FGameplayAttribute GetStrengthAttribute() const {return BasicAttributes->GetStrengthAttribute();}

	UFUNCTION(BlueprintCallable, Category = "SN|Attributes")
	float GetEndurance() const;

	UFUNCTION(BlueprintCallable, Category = "SN|Attributes")
	FGameplayAttribute GetEnduranceAttribute() const {return BasicAttributes->GetEnduranceAttribute();}

	UFUNCTION(BlueprintCallable, Category = "SN|Attributes")
	float GetFaith() const;

	UFUNCTION(BlueprintCallable, Category = "SN|Attributes")
	FGameplayAttribute GetFaithAttribute() const {return BasicAttributes->GetFaithAttribute();}
	
	/**
	 *
	 */
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	virtual void HealthChanged(const FOnAttributeChangeData& Data);
	virtual void MaxHealthChanged(const FOnAttributeChangeData& Data);
	virtual void ResourceChanged(const FOnAttributeChangeData& Data);
	virtual void MaxResourceChanged(const FOnAttributeChangeData& Data);
	virtual void ExperienceChanged(const FOnAttributeChangeData& Data);
	virtual void MaxExperienceChanged(const FOnAttributeChangeData& Data);
	virtual void CharacterLevelChanged(const FOnAttributeChangeData& Data);
	virtual void ArmourChanged(const FOnAttributeChangeData& Data);
	virtual void StrengthChanged(const FOnAttributeChangeData& Data);
	virtual void EnduranceChanged(const FOnAttributeChangeData& Data);
	virtual void FaithChanged(const FOnAttributeChangeData& Data);
	
	
protected:
	FDelegateHandle HealthChangedDelegateHandle;
	FDelegateHandle MaxHealthChangedDelegateHandle;
	FDelegateHandle ResourceChangedDelegateHandle;
	FDelegateHandle MaxResourceChangedDelegateHandle;
	FDelegateHandle ExperienceChangedDelegateHandle;
	FDelegateHandle MaxExperienceChangedDelegateHandle;
	FDelegateHandle CharacterLevelChangedDelegateHandle;
	FDelegateHandle ArmourChangedDelegateHandle;
	FDelegateHandle StrengthChangedDelegateHandle;
	FDelegateHandle EnduranceChangedDelegateHandle;
	FDelegateHandle FaithChangedDelegateHandle;
	
	UPROPERTY()
	USNAbilitySystemComponent* AbilitySystemComponent;

	UPROPERTY()
	const USNBasicAttributes* BasicAttributes;
	
	UPROPERTY()
	ESNDeathState DeathState;

public:	
	

		
};
