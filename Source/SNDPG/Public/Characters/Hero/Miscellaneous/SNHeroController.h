// Copyright 2023 Michal Oginski.

#pragma once

#include "CoreMinimal.h"
#include "Characters/SNCharacterBase.h"
#include "GameFramework/PlayerController.h"
#include "SNHeroController.generated.h"

class USNBasicAttributesComponent;
class USNHeroHUD;
class ASNHeroState;
class USNAbilitySystemComponent;
/**
 * 
 */
UCLASS()
class SNDPG_API ASNHeroController : public APlayerController
{
	GENERATED_BODY()
	
public:
	ASNHeroController();

	UFUNCTION(BlueprintCallable, Category = "SN|PlayerController")
	ASNHeroState* GetSNPlayerState() const;
	
	UFUNCTION(BlueprintCallable, Category = "SN|PlayerController")
	USNAbilitySystemComponent* GetSNAbilitySystemComponent() const;
	
	USNHeroHUD* GetHeroHUD();
	
	void CreateHeroHUD();

	void ShowFloatingNumber(float Amount, ASNCharacterBase* TargetCharacter, bool bIsDamage);
	
public:
	UPROPERTY(EditAnywhere, Category = "Hero|UI")
	TSubclassOf<class USNFloatingDmgNumberWComponent> DamageNumberClass;

protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Hero|UI")
	TSubclassOf<USNHeroHUD> HeroHUDClass;

	UPROPERTY(BlueprintReadWrite, Category = "Hero|UI")
	USNHeroHUD* HeroHUD;
	
protected:
	virtual void PreProcessInput(const float DeltaTime, const bool bGamePaused) override;
	virtual void PostProcessInput(const float DeltaTime, const bool bGamePaused) override;

	virtual void OnPossess(APawn* InPawn) override;
	
};
