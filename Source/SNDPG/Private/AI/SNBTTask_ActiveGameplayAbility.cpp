// Copyright 2026 out of sCope team - intrxx


#include "AI/SNBTTask_ActiveGameplayAbility.h"

#include "GAS/SNAbilitySystemComponent.h"
#include "Characters/Enemy/SNEnemy.h"
#include "Characters/Enemy/Miscellaneous/SNBaseAIController.h"

USNBTTask_ActiveGameplayAbility::USNBTTask_ActiveGameplayAbility()
{
	NodeName = TEXT("Activate Gameplay Ability");
}

EBTNodeResult::Type USNBTTask_ActiveGameplayAbility::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	if (ASNEnemy* Enemy = Cast<ASNBaseAIController>(OwnerComp.GetAIOwner())->EnemyOwner)
	{
		Enemy->GetEnemyAbilitySystemComponent()->TryActivateAbilityByClass(AbilityToActivate, true);

		return EBTNodeResult::Succeeded;
	}
	
	GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, TEXT("CAST FAILED"));
	return EBTNodeResult::Failed;
}
