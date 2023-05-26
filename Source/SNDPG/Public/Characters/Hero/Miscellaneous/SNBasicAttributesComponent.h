// Copyright 2023 Michal Oginski.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GAS/SNAbilitySystemComponent.h"
#include "GAS/Attributes/SNBasicAttributes.h"
#include "SNBasicAttributesComponent.generated.h"

class USNAbilitySystemComponent;
class USNBasicAttributes;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FBasicAttributes_DeathEvent, AActor*, OwningActor);

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
	
	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "SN|Attributes", Meta = (ExpandBoolAsExecs = "ReturnValue"))
	bool IsDeadOrDying() const {return (DeathState > ESNDeathState::NotDead);}
	
	// Begins the death sequence for the owner.
	virtual void StartDeath();

	// Ends the death sequence for the owner.
	virtual void FinishDeath();

	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "SN|Attributes")
	void LevelUpStrength(float Amount);
	
	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "SN|Attributes")
	void LevelUpEndurance(float Amount);
	
	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "SN|Attributes")
	void LevelUpFaith(float Amount);

	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "SN|Attributes")
	void LevelUpVitality(float Amount);
	
	
	
	/**
	 * Getters from Attributes
	 */
	
	UFUNCTION(BlueprintCallable, Category = "SN|Attributes")
	float GetHealth() const;
	
	UFUNCTION(BlueprintCallable, Category = "SN|Attributes")
	float GetMaxHealth() const;
	
	UFUNCTION(BlueprintCallable, Category = "SN|Attributes")
	FGameplayAttribute GetHealthAttribute() const;

	UFUNCTION(BlueprintCallable, Category = "SN|Attributes")
	FGameplayAttribute GetMaxHealthAttribute() const;
	
	UFUNCTION(BlueprintCallable, Category = "SN|Attributes")
	float GetResource() const;
	
	UFUNCTION(BlueprintCallable, Category = "SN|Attributes")
	float GetMaxResource() const;

	UFUNCTION(BlueprintCallable, Category = "SN|Attributes")
	FGameplayAttribute GetResourceAttribute() const;
	
	UFUNCTION(BlueprintCallable, Category = "SN|Attributes")
	FGameplayAttribute GetMaxResourceAttribute() const;

	UFUNCTION(BlueprintCallable, Category = "SN|Attributes")
	float GetCharacterLevel() const;

	UFUNCTION(BlueprintCallable, Category = "SN|Attributes")
	FGameplayAttribute GetCharacterLevelAttribute() const;
	
	UFUNCTION(BlueprintCallable, Category = "SN|Attributes")
	float GetExperience() const;
	
	UFUNCTION(BlueprintCallable, Category = "SN|Attributes")
	float GetMaxExperience() const;

	UFUNCTION(BlueprintCallable, Category = "SN|Attributes")
	FGameplayAttribute GetExperienceAttribute() const;

	UFUNCTION(BlueprintCallable, Category = "SN|Attributes")
	FGameplayAttribute GetMaxExperienceAttribute() const;

	UFUNCTION(BlueprintCallable, Category = "SN|Attributes")
	float GetArmour() const;

	UFUNCTION(BlueprintCallable, Category = "SN|Attributes")
	FGameplayAttribute GetArmourAttribute() const;

	UFUNCTION(BlueprintCallable, Category = "SN|Attributes")
	float GetStrength() const;

	UFUNCTION(BlueprintCallable, Category = "SN|Attributes")
	FGameplayAttribute GetStrengthAttribute() const;

	UFUNCTION(BlueprintCallable, Category = "SN|Attributes")
	float GetEndurance() const;

	UFUNCTION(BlueprintCallable, Category = "SN|Attributes")
	FGameplayAttribute GetEnduranceAttribute() const;

	UFUNCTION(BlueprintCallable, Category = "SN|Attributes")
	float GetFaith() const;

	UFUNCTION(BlueprintCallable, Category = "SN|Attributes")
	FGameplayAttribute GetFaithAttribute() const;

	UFUNCTION(BlueprintCallable, Category = "SN|Attributes")
	float GetHealing() const;

	UFUNCTION(BlueprintCallable, Category = "SN|Attributes")
	FGameplayAttribute GetHealingAttribute() const;

	UFUNCTION(BlueprintCallable, Category = "SN|Attributes")
	float GetLevelUpPoints() const;

	UFUNCTION(BlueprintCallable, Category = "SN|Attributes")
	FGameplayAttribute GetLevelUpPointsAttribute() const;

	UFUNCTION(BlueprintCallable, Category = "SN|Attributes")
	float GetVitality() const;

	UFUNCTION(BlueprintCallable, Category = "SN|Attributes")
	FGameplayAttribute GetVitalityAttribute() const;
	
	/**
	 *
	 */

public:

	// Delegate fired when the death sequence has started.
	UPROPERTY(BlueprintAssignable)
	FBasicAttributes_DeathEvent OnDeathStarted;

	// Delegate fired when the death sequence has finished.
	UPROPERTY(BlueprintAssignable)
	FBasicAttributes_DeathEvent OnDeathFinished;
	
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
	virtual void HealingChanged(const FOnAttributeChangeData& Data);
	virtual void LevelUpPointsChanged(const FOnAttributeChangeData& Data);
	virtual void VitalityChanged(const FOnAttributeChangeData& Data);

	void SubtractLevelUpPoints(float Amount);
	void AddLeveledUpAttribute(float Amount, FGameplayAttribute AttributeToAdd);

	virtual void HandleOutOfHealth(AActor* DamageInstigator, AActor* DamageCauser, const FGameplayEffectSpec& DamageEffectSpec, float DamageMagnitude);
	
	
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
	FDelegateHandle HealingChangedDelegateHandle;
	FDelegateHandle LevelUpPointsChangedDelegateHandle;
	FDelegateHandle VitalityChangedDelegateHandle;
	
	UPROPERTY()
	USNAbilitySystemComponent* AbilitySystemComponent;

	UPROPERTY()
	const USNBasicAttributes* BasicAttributes;
	
	UPROPERTY()
	ESNDeathState DeathState;

public:	
	

		
};
