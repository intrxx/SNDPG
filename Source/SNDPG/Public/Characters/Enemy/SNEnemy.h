// Copyright 2023 Michał Ogiński.

#pragma once

#include "CoreMinimal.h"
#include "Characters/SNCharacterBase.h"
#include "SNEnemy.generated.h"

class ASNWorldCollectable;
class USNLootSet;
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

	// Begins the death sequence for the character (disables collision, disables movement, etc...)
	UFUNCTION()
	virtual void OnDeathStarted(AActor* OwningActor);

	// Ends the death sequence for the character (detaches controller, destroys pawn, etc...)
	UFUNCTION()
	virtual void OnDeathFinished(AActor* OwningActor);

public:
	UPROPERTY(EditAnywhere, Category = "Enemy|UI")
	FVector HealthBarPosition = FVector(0.0f, 0.0f, 120.0f);

	UPROPERTY(BlueprintReadOnly, Category = "Enemy|Loot")
	TSubclassOf<ASNWorldCollectable> ItemToDrop;
	
protected:
	virtual void BeginPlay() override;

	void InitializeHealthBar();

	virtual void DestroyDueToDeath() override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enemy|Abilities")
	TObjectPtr<USNAbilitySet> AbilitySet;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enemy|LootSet")
	TObjectPtr<USNLootSet> LootSet;
	
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
