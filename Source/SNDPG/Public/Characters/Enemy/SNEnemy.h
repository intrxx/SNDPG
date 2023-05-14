// Copyright 2023 Michal Oginski.

#pragma once

#include "CoreMinimal.h"
#include "Characters/SNCharacterBase.h"
#include "GameplayEffectTypes.h"
#include "SNEnemy.generated.h"

class UWidgetComponent;
class USNHealthBarWidget;
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

	USNHealthBarWidget* GetHealthBarWidget() {return HealthBarWidget;}

public:
	UPROPERTY(EditAnywhere, Category = "Enemy|UI")
	FVector HealthBarPosition = FVector(0.0f, 0.0f, 120.0f);
	
protected:
	virtual void BeginPlay() override;

	void InitializeHealthBar();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enemy|Abilities")
	USNAbilitySet* AbilitySet;
	
	UPROPERTY()
	USNAbilitySystemComponent* EnemyAbilitySystemComponent;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Enemy|UI")
	TSubclassOf<USNHealthBarWidget> HealthBarWidgetClass;

	UPROPERTY()
	USNHealthBarWidget* HealthBarWidget;

	UPROPERTY()
	UWidgetComponent* HealthBarWidgetComponent;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Enemy|Attributes", Meta = (AllowPrivateAccess = "true"))
	USNBasicAttributesComponent* AttributesComponent;
	
};
