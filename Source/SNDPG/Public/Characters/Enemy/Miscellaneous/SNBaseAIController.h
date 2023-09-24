// Copyright 2023 Michał Ogiński.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "SNBaseAIController.generated.h"

class ASNEnemy;
class UBehaviorTreeComponent;
/**
 * 
 */
UCLASS()
class SNDPG_API ASNBaseAIController : public AAIController
{
	GENERATED_BODY()

public:
	ASNBaseAIController();

	UPROPERTY(BlueprintReadWrite, Category = "SN|AI")
	TObjectPtr<ASNEnemy> EnemyOwner;

protected:
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override;

	UFUNCTION()
	void OnStimulusChange(AActor* UpdateActor, struct FAIStimulus Stimulus);

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SN|AI", meta = (AllowPrivateAccess = true))
	TObjectPtr<UBehaviorTree> BehaviorTree;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SN|AI", meta = (AllowPrivateAccess = true))
	TObjectPtr<UAIPerceptionComponent> PerceptionComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SN|AI", meta = (AllowPrivateAccess = true))
	TObjectPtr<UBlackboardComponent> BlackboardComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SN|AI", meta = (AllowPrivateAccess = true))
	TObjectPtr<UBehaviorTreeComponent> BehaviorTreeComp;
	
};
