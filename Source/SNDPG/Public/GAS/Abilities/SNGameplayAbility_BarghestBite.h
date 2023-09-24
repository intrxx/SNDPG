// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/SNGameplayAbility.h"
#include "SNGameplayAbility_BarghestBite.generated.h"

class ASNEnemy;
/**
 * 
 */
UCLASS()
class SNDPG_API USNGameplayAbility_BarghestBite : public USNGameplayAbility
{
	GENERATED_BODY()

public:
	USNGameplayAbility_BarghestBite(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable, Category = "SN|Ability|BarghestBite")
	ASNEnemy* GetBarghestFromActorInfo() const;

	UFUNCTION(BlueprintCallable, Category = "SN|BarghestBite")
	void SetDamageAndMontage();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "SN|BarghestBite")
	TObjectPtr<UAnimMontage> LowLifeAnimMontage;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "SN|BarghestBite")
	TObjectPtr<UAnimMontage> HighLifeAnimMontage;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "SN|BarghestBite")
	float LowLifeDamage = 45.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "SN|BarghestBite")
	float HighLifeDamage = 30.0f;
	
protected:
	UPROPERTY(BlueprintReadOnly, Category = "SN|BarghestBite", meta = (AllowPrivateAccess = true))
	TObjectPtr<UAnimMontage> AnimMontageToUse;

	UPROPERTY(BlueprintReadOnly, Category = "SN|BarghestBite", meta = (AllowPrivateAccess = true))
	float DamageToUse = 0.0f;
};
