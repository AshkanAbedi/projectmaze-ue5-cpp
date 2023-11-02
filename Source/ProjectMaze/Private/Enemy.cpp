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
	MoveToPatrolTarget(FirstPatrolTarget);
}

void AEnemy::SelectRandomPatrolTarget()
{
	const int32 NumPatrolTargets = PatrolTargets.Num();
	if (NumPatrolTargets > 0)
	{
		const int32 RandomIndex = FMath::RandRange(0, NumPatrolTargets - 1);
		NextPatrolTarget = PatrolTargets[RandomIndex];
		if(GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Next Patrol Target: %s"), *NextPatrolTarget->GetName()));
	}
}

bool AEnemy::IsInPatrolTargetRadius() const
{
	const double Distance = (GetActorLocation() - CurrentPatrolTarget->GetActorLocation()).Size();
	GEngine->AddOnScreenDebugMessage(1, 5.f, FColor::Red, FString::Printf(TEXT("Distance: %f"), Distance));
	return Distance <= PatrolTargetRadius;
}

void AEnemy::ChoosePatrolTarget()
{
	const float WaitTime = FMath::RandRange(WaitMin, WaitMax);
	GetWorldTimerManager().SetTimer(WaitTimerHandle, this, &AEnemy::WaitTimeFinished, WaitTime, false, 3.f);
}

void AEnemy::WaitTimeFinished()
{
	MoveToPatrolTarget(NextPatrolTarget);
}

void AEnemy::MoveToPatrolTarget(AActor* PatrolTarget)
{
	if (AIController)
		AIController->MoveToActor(PatrolTarget, 20.f);
		CurrentPatrolTarget = PatrolTarget;
}

void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (IsInPatrolTargetRadius())
		SelectRandomPatrolTarget();
		WaitTimeFinished();
}

void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

