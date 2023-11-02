// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AIController.h"

AEnemy::AEnemy()
{
	PrimaryActorTick.bCanEverTick = true;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->MaxWalkSpeed = 110.f;
}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	AIController = Cast<AAIController>(GetController());
	SelectRandomPatrolTarget();
}

void AEnemy::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	GetWorldTimerManager().ClearTimer(CheckDistanceTimerHandle);
	GetWorldTimerManager().ClearTimer(WaitTimerHandle);
}

void AEnemy::SelectRandomPatrolTarget()
{
	const int32 NumPatrolTargets = PatrolTargets.Num();
	if (NumPatrolTargets > 0)
	{
		const int32 RandomIndex = FMath::RandRange(0, NumPatrolTargets - 1);
		NextPatrolTarget = PatrolTargets[RandomIndex];
		MoveToPatrolTarget(NextPatrolTarget);
		GetWorldTimerManager().ClearTimer(WaitTimerHandle);
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Next Patrol Target: %s"), *NextPatrolTarget->GetName()));
	}
}

void AEnemy::IsInPatrolTargetRadius()
{
	const double Distance = (GetActorLocation() - CurrentPatrolTarget->GetActorLocation()).Size();
	GEngine->AddOnScreenDebugMessage(1, 5.f, FColor::Red, FString::Printf(TEXT("Distance: %f"), Distance));
	if (Distance <= PatrolTargetRadius)
		GetWorldTimerManager().ClearTimer(CheckDistanceTimerHandle);
		GetWorld()->GetTimerManager().SetTimer(WaitTimerHandle, this, &AEnemy::SelectRandomPatrolTarget, FMath::RandRange(WaitMin, WaitMax), false);
		bIsInPatrolTargetRadius = true;
}

void AEnemy::MoveToPatrolTarget(AActor* PatrolTarget)
{
	if (AIController)
		AIController->MoveToActor(PatrolTarget, 20.f);
		GetWorldTimerManager().SetTimer(CheckDistanceTimerHandle, this, &AEnemy::IsInPatrolTargetRadius, 0.1, true);
		CurrentPatrolTarget = PatrolTarget;
}

void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

