// Copyright 2023 Michal Oginski.


#include "ActorComponents/SNCombatComponent.h"

#include "Kismet/KismetSystemLibrary.h"

USNCombatComponent::USNCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = true;
	
}

void USNCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if(bTracing)
	{
		TraceTick();
	}
}

void USNCombatComponent::StartTrace()
{
	RefreshIgnoredActors();

	bTracing = true;

	PreviousStarts.SetNum(TrackedMeshes.Num());
	PreviousEnds.SetNum(TrackedMeshes.Num());
	int32 Index = 0;
	for(const auto& Entry : TrackedMeshes)
	{
		UPrimitiveComponent* Mesh = Entry;
		if(!IsValid(Mesh))
		{
			UE_LOG(LogTemp, Error, TEXT("Mesh is invalid couldn't start tracing!"));
			RemoveTraceMesh(Mesh);
			continue;
		}

		FVector WorldStart;
		FVector WorldEnd;
		GetStartEndLocationByMesh(Mesh, WorldStart, WorldEnd);
		PreviousStarts[Index] = WorldStart;
		PreviousEnds[Index] = WorldEnd;

		++Index;
	}
	OnStartAttack.Broadcast();
	TraceTick();
}

void USNCombatComponent::EndTrace()
{
	bTracing = false;
	
	PreviousStarts.Empty();
	PreviousEnds.Empty();

	OnEndAttack.Broadcast();
}

void USNCombatComponent::AddTraceMesh(UPrimitiveComponent* Mesh)
{
	if(!IsValid(Mesh))
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid mesh passed, Nothing will be added to TrackedMeshes"));
		return;
	}
	TrackedMeshes.Add(Mesh);
}

void USNCombatComponent::RemoveTraceMesh(UPrimitiveComponent* Mesh)
{
	if(!TrackedMeshes.Find(Mesh))
	{
		UE_LOG(LogTemp, Error, TEXT("Couldn't find Mesh to remove!"));
		return;
	}

	TrackedMeshes.Remove(Mesh);
}

void USNCombatComponent::RefreshIgnoredActors()
{
	GetOwner()->GetAttachedActors(IgnoredActors);
}

void USNCombatComponent::GetAllWeapons(TArray<AActor*>& OutWeapons)
{
	for(const auto& TrackedMesh : TrackedMeshes)
	{
		OutWeapons.Add(TrackedMesh->GetOwner());
	}
}

void USNCombatComponent::GetStartEndLocationByMesh(const UPrimitiveComponent* Mesh, FVector& OutWorldStart,
	FVector& OutWorldEnd) const
{
	OutWorldStart = Mesh->GetSocketLocation(TraceLineStartSocket);
	OutWorldEnd = Mesh->GetSocketLocation(TraceLineEndSocket);
}

void USNCombatComponent::TraceTick()
{
	int32 Index = 0;
	for(const auto& Entry : TrackedMeshes)
	{
		UPrimitiveComponent* Mesh = Entry;

		DoCollisionTest_ComplexLineTrace(Index, Mesh, TraceColor1, TraceColor2);

		++Index;
	}
}

void USNCombatComponent::GetPreviousPoints(const int32 Index, FVector& OutStart, FVector& OutEnd) const
{
	if(Index < 0 || Index >= PreviousStarts.Num() || Index >= PreviousEnds.Num())
	{
		return;
	}

	OutStart = PreviousStarts[Index];
	OutEnd = PreviousEnds[Index];
}

void USNCombatComponent::CalculateTracePoint(const int32 Index, const FVector& Start, const FVector& End,
	FVector& OutPoint) const
{
	OutPoint = Start + Index * ((End - Start) / TracePrecision);
}

void USNCombatComponent::HandleOnAttackHitEvent(const bool bHit, const TArray<FHitResult>& HitResults,
	UPrimitiveComponent* Mesh) const
{
	if(!bHit)
	{
		return;
	}

	for(const auto& HitResult : HitResults)
	{
		OnAttackHitEvent.Broadcast(HitResult, Mesh);
	}
}

void USNCombatComponent::DoCollisionTest_SimpleLineTrace(UPrimitiveComponent* Mesh, const FColor DebugTraceColor) const
{
	TArray<FHitResult> HitResults;

	FVector Start;
	FVector End;
	GetStartEndLocationByMesh(Mesh, Start, End);

	const bool bHit = UKismetSystemLibrary::LineTraceMulti(this, Start, End, TraceChannel, bTraceComplex, IgnoredActors,
		bEnableDebug ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None, HitResults, true, DebugTraceColor);

	HandleOnAttackHitEvent(bHit, HitResults, Mesh);
}

void USNCombatComponent::DoCollisionTest_ComplexLineTrace(const int32 TrackedMeshIndex, UPrimitiveComponent* Mesh,
                                                          const FColor TraceColour1, const FColor TraceColour2)
{
	DoCollisionTest_SimpleLineTrace(Mesh, TraceColour2);

	for(int32 Index = 0; Index <= TracePrecision; ++Index)
	{
		TArray<FHitResult> HitResults;

		// Points in between calculation
		FVector TempStart;
		FVector TempEnd;

		FVector Start;
		GetPreviousPoints(TrackedMeshIndex, TempStart, TempEnd);
		CalculateTracePoint(Index, TempStart, TempEnd, Start);

		FVector End;
		GetStartEndLocationByMesh(Mesh, TempStart, TempEnd);
		CalculateTracePoint(Index, TempStart, TempEnd, End);

		const bool bHit = UKismetSystemLibrary::LineTraceMulti(this, Start, End, TraceChannel, bTraceComplex, IgnoredActors,
			bEnableDebug ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None,HitResults,true,TraceColour1);

		HandleOnAttackHitEvent(bHit, HitResults, Mesh);
	}

	// Set next frame targets
	PreviousStarts.SetNum(TrackedMeshes.Num());
	PreviousEnds.SetNum(TrackedMeshes.Num());

	FVector Start;
	FVector End;
	GetStartEndLocationByMesh(Mesh, Start, End);
	PreviousStarts[TrackedMeshIndex] = Start;
	PreviousEnds[TrackedMeshIndex] = End;
}

