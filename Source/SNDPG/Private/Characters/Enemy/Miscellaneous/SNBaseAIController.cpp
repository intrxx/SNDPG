// Copyright 2023 Michał Ogiński.


#include "Characters/Enemy/Miscellaneous/SNBaseAIController.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Characters/Enemy/SNEnemy.h"
#include "Characters/Hero/SNHero.h"
#include "Perception/AIPerceptionComponent.h"

ASNBaseAIController::ASNBaseAIController()
{
	BlackboardComp = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackBoardComp"));
	BehaviorTreeComp = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviourTreeComp"));
}

void ASNBaseAIController::BeginPlay()
{
	Super::BeginPlay();

	if(BehaviorTree)
	{
		RunBehaviorTree(BehaviorTree);
		BehaviorTreeComp->StartTree(*BehaviorTree);
	}
}

void ASNBaseAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	EnemyOwner = Cast<ASNEnemy>(InPawn);

	PerceptionComp = FindComponentByClass<UAIPerceptionComponent>();

	if(Blackboard && BehaviorTree)
	{
		Blackboard->InitializeBlackboard(*BehaviorTree->BlackboardAsset);
	}

	if(PerceptionComp)
	{
		PerceptionComp->OnTargetPerceptionUpdated.AddDynamic(this, &ThisClass::OnStimulusChange);
	}
}

void ASNBaseAIController::OnStimulusChange(AActor* UpdateActor, FAIStimulus Stimulus)
{
	ASNHero* Hero = Cast<ASNHero>(UpdateActor);
	if(!Hero)
	{
		return;
	}

	if(Stimulus.IsActive())
	{
		Blackboard->SetValueAsBool(FName("bIsPlayerSeen"), true);
		Blackboard->SetValueAsObject(FName("Player"), Hero);
	}
	else
	{
		Blackboard->SetValueAsBool(FName("bIsPlayerSeen"), false);
		Blackboard->ClearValue(FName("Player"));
	}
}
