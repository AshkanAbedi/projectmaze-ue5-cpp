// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CharacterStates.h"
#include "Enemy.generated.h"

class APlayerCharacter;
class AAIController;
class UPawnSensingComponent;

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

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Pawn Sensing") TObjectPtr<APlayerCharacter> PlayerCharacter;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Pawn Sensing") TObjectPtr<UPawnSensingComponent> PawnSensingComponent;

	UFUNCTION() void OnSeePawn(APawn* Pawn);
	UFUNCTION() void IgnorePawn();

	FTimerHandle AlertTimerHandle;
	
#pragma endregion PawnSensing
	
#pragma region Navigation

	UPROPERTY() AAIController* AIController;
	UPROPERTY(EditInstanceOnly, Category = "AI Navigation") TArray<AActor*> PatrolTargets;
	UPROPERTY(EditInstanceOnly, Category = "AI Navigation") TObjectPtr<AActor> NextPatrolTarget;
	UPROPERTY(EditInstanceOnly, Category = "AI Navigation") TObjectPtr<AActor> CurrentPatrolTarget;
	UPROPERTY(EditInstanceOnly, Category = "AI Navigation") float PatrolTargetRadius;
	UPROPERTY(EditInstanceOnly, Category = "AI Navigation") float WaitMin;
	UPROPERTY(EditInstanceOnly, Category = "AI Navigation") float WaitMax;
	
	void MoveToPatrolTarget(AActor* PatrolTarget);
	void SelectRandomPatrolTarget();
	void IsInPatrolTargetRadius();

	FTimerHandle WaitTimerHandle;
	FTimerHandle CheckDistanceTimerHandle;

#pragma endregion Navigation

private:
	
};
