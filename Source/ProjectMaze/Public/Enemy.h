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



protected:
	virtual void BeginPlay() override;

#pragma region Navigation

	UPROPERTY() AAIController* AIController;
	UPROPERTY() FTimerHandle WaitTimerHandle;
	UPROPERTY(EditInstanceOnly, Category = "AI Navigation") TArray<AActor*> PatrolTargets;
	UPROPERTY(EditInstanceOnly, Category = "AI Navigation") AActor* FirstPatrolTarget;
	UPROPERTY(EditInstanceOnly, Category = "AI Navigation") AActor* NextPatrolTarget;
	UPROPERTY(EditInstanceOnly, Category = "AI Navigation") AActor* CurrentPatrolTarget;
	UPROPERTY(EditInstanceOnly, Category = "AI Navigation") float PatrolTargetRadius;
	UPROPERTY(EditInstanceOnly, Category = "AI Navigation") float WaitMin;
	UPROPERTY(EditInstanceOnly, Category = "AI Navigation") float WaitMax;
	
	UFUNCTION() void MoveToPatrolTarget(AActor* PatrolTarget);
	UFUNCTION() void SelectRandomPatrolTarget();
	UFUNCTION() bool IsInPatrolTargetRadius() const;
	UFUNCTION() void ChoosePatrolTarget();
	UFUNCTION() void WaitTimeFinished();

#pragma endregion Navigation

private:
	
};
