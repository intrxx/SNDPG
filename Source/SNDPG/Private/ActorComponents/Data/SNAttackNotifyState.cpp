// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponents/Data/SNAttackNotifyState.h"

#include "ActorComponents/SNCombatComponent.h"

void USNAttackNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                       float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	if(MeshComp)
	{
		if(const AActor* Owner = MeshComp->GetOwner())
		{
			USNCombatComponent* CombatComponent = Owner->FindComponentByClass<USNCombatComponent>();
			if(!IsValid(CombatComponent))
			{
				UE_LOG(LogTemp, Error, TEXT("CombatComponent is invalid!"));
				return;
			}

			CombatComponent->StartTrace();
		}
	}
}

void USNAttackNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	if(MeshComp)
	{
		if(const AActor* Owner = MeshComp->GetOwner())
		{
			USNCombatComponent* CombatComponent = Owner->FindComponentByClass<USNCombatComponent>();
			if(!IsValid(CombatComponent))
			{
				UE_LOG(LogTemp, Error, TEXT("CombatComponent is invalid!"));
				return;
			}
			
			CombatComponent->EndTrace();
		}
	}
}
