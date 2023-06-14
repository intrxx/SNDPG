// Copyright 2023 Michal Oginski.

#pragma once

#include "CoreMinimal.h"
#include "Characters/SNCharacterBase.h"
#include "GameFramework/PlayerController.h"
#include "SNHeroController.generated.h"

class USNCharacterStatusWidget;
class USNInGameMenu;
class USNInventoryWidget;
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

	UFUNCTION(BlueprintCallable, Category = "SN|PlayerController|UI")
	USNHeroHUD* GetHeroHUD() {return HeroHUD;}

	UFUNCTION(BlueprintCallable, Category = "SN|PlayerController|UI")
	USNInventoryWidget* GetInventoryUI() {return InventoryWidget;}

	UFUNCTION(BlueprintCallable, Category = "SN|PlayerController|UI")
	USNInGameMenu* GetInGameMenuUI() {return InGameMenuWidget;}

	UFUNCTION(BlueprintCallable, Category = "SN|PlayerController|UI")
	USNCharacterStatusWidget* GetCharacterStatusUI() {return CharacterStatusWidget;}
	
	void CreateHeroHUD();
	void CreateInventoryUI(const USNBasicAttributesComponent* AttributesComp);
	void CreateInGameMenuUI();
	void CreateCharacterStatusUI(const USNBasicAttributesComponent* AttributesComp);

	void ShowFloatingNumber(float Amount, ASNCharacterBase* TargetCharacter);
	
public:
	UPROPERTY(EditAnywhere, Category = "Hero|UI")
	TSubclassOf<class USNFloatingDmgNumberWComponent> DamageNumberClass;

protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Hero|UI")
	TSubclassOf<USNHeroHUD> HeroHUDClass;

	UPROPERTY(BlueprintReadWrite, Category = "Hero|UI")
	USNHeroHUD* HeroHUD;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Hero|UI")
	TSubclassOf<USNInventoryWidget> InventoryWidgetClass;

	UPROPERTY(BlueprintReadWrite, Category = "Hero|UI")
	USNInventoryWidget* InventoryWidget;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Hero|UI")
	TSubclassOf<USNInGameMenu> InGameMenuWidgetClass;

	UPROPERTY(BlueprintReadWrite, Category = "Hero|UI")
	USNInGameMenu* InGameMenuWidget;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Hero|UI")
	TSubclassOf<USNCharacterStatusWidget> CharacterStatusWidgetClass;

	UPROPERTY(BlueprintReadWrite, Category = "Hero|UI")
	USNCharacterStatusWidget* CharacterStatusWidget;
	
protected:
	virtual void PreProcessInput(const float DeltaTime, const bool bGamePaused) override;
	virtual void PostProcessInput(const float DeltaTime, const bool bGamePaused) override;

	virtual void OnPossess(APawn* InPawn) override;
	
};
