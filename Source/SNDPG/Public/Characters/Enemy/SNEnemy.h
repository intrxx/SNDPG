// Copyright 2023 Michal Oginski.

#pragma once

#include "CoreMinimal.h"
#include "Characters/SNCharacterBase.h"
#include "GameplayEffectTypes.h"
#include "SNEnemy.generated.h"

class USNBasicAttributes;
class USNBasicAttributesComponent;
class USNAbilitySet;
/**
 * 
 */
UCLASS()
class SNDPG_API ASNEnemy : public ASNCharacterBase
{
	GENERATED_BODY()

public:
	ASNEnemy(const FObjectInitializer& ObjectInitializer);

	USNAbilitySystemComponent* GetEnemyAbilitySystemComponent() const;

	
protected:
	virtual void BeginPlay() override;

	// Attribute changed callbacks
	virtual void HealthChanged(const FOnAttributeChangeData& Data);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SN|Abilities")
	USNAbilitySet* AbilitySet;
	
	FDelegateHandle HealthChangedDelegateHandle;
	
	UPROPERTY()
	USNAbilitySystemComponent* EnemyAbilitySystemComponent;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SN|Character", Meta = (AllowPrivateAccess = "true"))
	USNBasicAttributesComponent* AttributesComponent;
	
};
