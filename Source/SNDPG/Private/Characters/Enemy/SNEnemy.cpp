// Copyright 2023 Michał Ogiński.


#include "Characters/Enemy/SNEnemy.h"
#include "GAS/SNAbilitySet.h"
#include "UI/EnemyUI/SNHealthBarWidget.h"
#include "ActorComponents/SNBasicAttributesComponent.h"
#include "Components/WidgetComponent.h"
#include "GAS/SNAbilitySystemComponent.h"

ASNEnemy::ASNEnemy(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	EnemyAbilitySystemComponent = CreateDefaultSubobject<USNAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	EnemyAbilitySystemComponent->SetIsReplicated(true);
	EnemyAbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);
	
	AbilitySystemComponent = EnemyAbilitySystemComponent;
	
	AttributesComponent = CreateDefaultSubobject<USNBasicAttributesComponent>(TEXT("BasicAttributesComponent"));
	AttributesComponent->OnDeathStarted.AddDynamic(this, &ThisClass::OnDeathStarted);
	AttributesComponent->OnDeathFinished.AddDynamic(this, &ThisClass::OnDeathFinished);
	
	HealthBarWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthBarWidgetComponent"));
	HealthBarWidgetComponent->SetupAttachment(RootComponent);
	HealthBarWidgetComponent->SetRelativeLocation(HealthBarPosition);
	HealthBarWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
	HealthBarWidgetComponent->SetDrawSize(FVector2D(500, 500));
}

USNAbilitySystemComponent* ASNEnemy::GetEnemyAbilitySystemComponent() const
{
	return EnemyAbilitySystemComponent;
}

void ASNEnemy::OnDeathStarted(AActor* OwningActor)
{
	DisableMovementAndCollision();
}

void ASNEnemy::OnDeathFinished(AActor* OwningActor)
{
	GetWorld()->GetTimerManager().SetTimerForNextTick(this, &ThisClass::DestroyDueToDeath);
}

void ASNEnemy::BeginPlay()
{
	Super::BeginPlay();

	if(AbilitySystemComponent.IsValid())
	{
		AbilitySystemComponent->InitAbilityActorInfo(this,this);
		AbilitySet->GiveToAbilitySystem(AbilitySystemComponent.Get(), nullptr, nullptr);
		AttributesComponent->InitializeWithAbilitySystem(AbilitySystemComponent.Get());

		InitializeHealthBar();
	}
}

void ASNEnemy::InitializeHealthBar()
{
	if(HealthBarWidget || !AbilitySystemComponent.Get())
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to initialize Widget on: %s"), *GetOwner()->GetName())
		return;
	}

	if(HealthBarWidgetClass)
	{
		HealthBarWidget = CreateWidget<USNHealthBarWidget>(GetWorld(), HealthBarWidgetClass);
		if(HealthBarWidget && HealthBarWidgetClass)
		{
			HealthBarWidgetComponent->SetWidget(HealthBarWidget);

			if(AttributesComponent)
			{
				HealthBarWidget->SetHealthPercentage(AttributesComponent->GetHealth() / AttributesComponent->GetMaxHealth());
			}
		}
	}
}

void ASNEnemy::DestroyDueToDeath()
{
	Super::DestroyDueToDeath();
	
	AttributesComponent->UninitializeFromAbilitySystem();
}



