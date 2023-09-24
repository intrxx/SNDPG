// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "SNBTTask_ActiveGameplayAbility.generated.h"

class UGameplayAbility;
/**
 * 
 */
UCLASS()
class SNDPG_API USNBTTask_ActiveGameplayAbility : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	USNBTTask_ActiveGameplayAbility();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AbilityToActive")
	TSubclassOf<UGameplayAbility> AbilityToActivate;
};
