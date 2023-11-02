// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Enemy.generated.h"

class AAIController;

UCLASS()
class PROJECTMAZE_API AEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	AEnemy();
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void Tick(float DeltaTime) override;

	bool bIsInPatrolTargetRadius = false;
	FTimerHandle WaitTimerHandle;
	FTimerHandle CheckDistanceTimerHandle;
	
protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

#pragma region Navigation

	UPROPERTY() AAIController* AIController;
	UPROPERTY(EditInstanceOnly, Category = "AI Navigation") TArray<AActor*> PatrolTargets;
	UPROPERTY(EditInstanceOnly, Category = "AI Navigation") AActor* NextPatrolTarget;
	UPROPERTY(EditInstanceOnly, Category = "AI Navigation") AActor* CurrentPatrolTarget;
	UPROPERTY(EditInstanceOnly, Category = "AI Navigation") float PatrolTargetRadius;
	UPROPERTY(EditInstanceOnly, Category = "AI Navigation") float WaitMin;
	UPROPERTY(EditInstanceOnly, Category = "AI Navigation") float WaitMax;

	void CheckTimer();
	void MoveToPatrolTarget(AActor* PatrolTarget);
	void SelectRandomPatrolTarget();
	void IsInPatrolTargetRadius();

#pragma endregion Navigation

private:
	
};
