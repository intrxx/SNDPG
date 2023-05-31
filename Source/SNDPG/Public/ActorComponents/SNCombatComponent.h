// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SNCombatComponent.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAttackHitDelegate, FHitResult, Hit, UPrimitiveComponent*, Mesh);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAttackStartDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAttackEndDelegate);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SNDPG_API USNCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintPure, Category = "Combat")
	static USNCombatComponent* FindCombatComponent(const AActor* Actor) {return (Actor ? Actor->FindComponentByClass<USNCombatComponent>() : nullptr);}
	
	// TRACE LINE SET UP OPTIONS
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat|Setup")
	FName TraceLineStartSocket = "Start";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat|Setup")
	FName TraceLineEndSocket = "End";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat|Setup")
	TEnumAsByte<ETraceTypeQuery> TraceChannel = UEngineTypes::ConvertToTraceType(ECC_Visibility);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat|Setup")
	bool bTraceComplex = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat|Setup")
	int32 TracePrecision = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat|Setup")
	float TraceSize = 5.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Combat|Setup")
	TArray<AActor*> IgnoredActors;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Combat|Setup")
	TArray<UPrimitiveComponent*> TrackedMeshes;

	// TRACE LINE DEBUG OPTIONS
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Combat|Debug")
	bool bEnableDebug = false;

	UPROPERTY(EditDefaultsOnly,Category = "Combat|Debug")
	FColor TraceColor1 = FColor::Green;

	UPROPERTY(EditDefaultsOnly, Category = "Combat|Debug")
	FColor TraceColor2 = FColor::Red;

	// EVENTS
	UPROPERTY(BlueprintAssignable, Category = "Combat|Delegates")
	FOnAttackHitDelegate OnAttackHitEvent;

	UPROPERTY(BlueprintAssignable, Category = "Combat|Delegates")
	FOnAttackStartDelegate OnStartAttack;

	UPROPERTY(BlueprintAssignable, Category = "Combat|Delegates")
	FOnAttackEndDelegate OnEndAttack;

public:
	USNCombatComponent();
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	UFUNCTION(BlueprintCallable, Category = "Combat")
	void StartTrace();

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void EndTrace();

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void SetupDebug(UPARAM(DisplayName="Debug") const bool bInDebug);

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void AddTraceMesh(UPrimitiveComponent* Mesh);

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void RemoveTraceMesh(UPrimitiveComponent* Mesh);

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void ClearAllMeshes();

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void RefreshIgnoredActors();

	UFUNCTION(BlueprintPure, Category = "Combat")
	void GetAllWeapons(UPARAM(DisplayName="Weapons") TArray<AActor*>& OutWeapons);

private:
	bool bTracing = false;

	UPROPERTY()
	TArray<FVector> PreviousStarts;

	UPROPERTY()
	TArray<FVector> PreviousEnds;

private:
	void GetStartEndLocationByMesh(const UPrimitiveComponent* Mesh, UPARAM(DisplayName = "WorldStart") FVector& OutWorldStart, UPARAM(DisplayName = "WorldEnd") FVector& OutWorldEnd) const;
	void TraceTick();
	void GetPreviousPoints(const int32 Index, UPARAM(DisplayName = "Start") FVector& OutStart, UPARAM(DisplayName = "End") FVector& OutEnd) const;
	void CalculateTracePoint(const int32 Index, const FVector& Start, const FVector& End, UPARAM(DisplayName = "Point") FVector& OutPoint) const;
	void HandleOnAttackHitEvent(const bool bHit, const TArray<FHitResult>& HitResults, UPrimitiveComponent* Mesh) const;
	
	void DoCollisionTest_SimpleLineTrace(UPrimitiveComponent* Mesh, const FColor DebugTraceColor) const;
	void DoCollisionTest_ComplexLineTrace(const int32 TrackedMeshIndex, UPrimitiveComponent* Mesh, const FColor TraceColor1, const FColor TraceColor2);
};
