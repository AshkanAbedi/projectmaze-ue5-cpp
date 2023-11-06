// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CharacterStates.h"
#include "Enemy.generated.h"

#pragma region Forward Declarations

class AAIController;
class UPawnSensingComponent;

#pragma endregion Forward Declarations

UCLASS()
class PROJECTMAZE_API AEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	AEnemy();
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void Tick(float DeltaTime) override;
	EEnemyState EnemyState;

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

#pragma region PawnSensing
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Pawn Sensing") TObjectPtr<UPawnSensingComponent> PawnSensingComponent;

	UFUNCTION() void OnSeePawn(APawn* Pawn);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pawn Sensing") float InnerChaseRadius;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pawn Sensing") float OuterChaseRadius;
	
#pragma endregion PawnSensing
	
#pragma region Navigation

	UPROPERTY() AAIController* AIController;
	UPROPERTY(EditInstanceOnly, Category = "AI Navigation") TArray<AActor*> PatrolTargets;
	UPROPERTY(EditInstanceOnly, Category = "AI Navigation") TObjectPtr<AActor> NextTarget;
	UPROPERTY(EditInstanceOnly, Category = "AI Navigation") TObjectPtr<AActor> CurrentTarget;
	UPROPERTY(EditInstanceOnly, Category = "AI Navigation") float PatrolTargetRadius;
	UPROPERTY(EditInstanceOnly, Category = "AI Navigation") float WaitMin;
	UPROPERTY(EditInstanceOnly, Category = "AI Navigation") float WaitMax;
	
	void MoveToTarget(AActor* Target);
	void Patrolling();
	void CheckPatrolTargetDistance();

#pragma endregion Navigation

#pragma region TimerHandles
	
	FTimerHandle CheckDistanceTimerHandle;
	FTimerHandle WaitTimerHandle;
	FTimerHandle ChaseTimerHandle;
	
#pragma endregion TimerHandles

private:
	
};
