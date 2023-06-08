// Copyright 2023 Michal Oginski.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "SNAttackNotifyState.generated.h"

/**
 * 
 */
UCLASS()
class SNDPG_API USNAttackNotifyState : public UAnimNotifyState
{
	GENERATED_BODY()

public:
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};
