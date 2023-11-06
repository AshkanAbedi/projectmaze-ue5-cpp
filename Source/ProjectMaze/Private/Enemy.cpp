// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AIController.h"
#include "Kismet/GameplayStatics.h"
#include "Perception/PawnSensingComponent.h"

AEnemy::AEnemy()
{
	PrimaryActorTick.bCanEverTick = true;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->MaxWalkSpeed = 110.f;

	PawnSensingComponent = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("Pawn Sensing Component"));
	PawnSensingComponent->SightRadius = 1500.f;
	PawnSensingComponent->SensingInterval = 0.25f;
	PawnSensingComponent->SetPeripheralVisionAngle(45.f);
}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	AIController = Cast<AAIController>(GetController());
	EnemyState = EEnemyState::EES_Patrolling;

	Patrolling();
	PawnSensingComponent->OnSeePawn.AddDynamic(this, &AEnemy::OnSeePawn);
}

void AEnemy::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	GetWorldTimerManager().ClearTimer(CheckDistanceTimerHandle);
	GetWorldTimerManager().ClearTimer(WaitTimerHandle);
	GetWorldTimerManager().ClearTimer(ChaseTimerHandle);
}

void AEnemy::Patrolling()
{
	const int32 NumPatrolTargets = PatrolTargets.Num();
	if (NumPatrolTargets > 0)
	{
		const int32 RandomIndex = FMath::RandRange(0, NumPatrolTargets - 1);
		NextTarget = PatrolTargets[RandomIndex];
		GetCharacterMovement()->MaxWalkSpeed = 110.f;
		MoveToTarget(NextTarget);
		EnemyState = EEnemyState::EES_Patrolling;
		GetWorldTimerManager().SetTimer(CheckDistanceTimerHandle, this, &AEnemy::CheckPatrolTargetDistance, 0.1, true);
		GetWorldTimerManager().ClearTimer(WaitTimerHandle);
		GEngine->AddOnScreenDebugMessage(1, 5.f, FColor::Red, FString::Printf(TEXT("Next Patrol Target: %s"), *NextTarget->GetName()));
	}
}

void AEnemy::CheckPatrolTargetDistance()
{
	const double Distance = (GetActorLocation() - CurrentTarget->GetActorLocation()).Size();
	GEngine->AddOnScreenDebugMessage(2, 5.f, FColor::Red, FString::Printf(TEXT("Distance: %f"), Distance));
	
	if (Distance <= PatrolTargetRadius)
		GetWorldTimerManager().ClearTimer(CheckDistanceTimerHandle);
		GetWorld()->GetTimerManager().SetTimer(WaitTimerHandle, this, &AEnemy::Patrolling, FMath::RandRange(WaitMin, WaitMax), false);
}

// ReSharper disable once CppMemberFunctionMayBeConst
void AEnemy::OnSeePawn(APawn* Pawn)
{
	if (Pawn->ActorHasTag(FName("Player")) && EnemyState != EEnemyState::EES_Chasing)
		EnemyState = EEnemyState::EES_Chasing;
		GetCharacterMovement()->MaxWalkSpeed = 350.f;
		GetWorldTimerManager().ClearTimer(CheckDistanceTimerHandle);
		GetWorldTimerManager().ClearTimer(WaitTimerHandle);
		MoveToTarget(Pawn);
		if (!PawnSensingComponent->CouldSeePawn(Pawn, true) && EnemyState == EEnemyState::EES_Chasing)
			Patrolling();
}

void AEnemy::MoveToTarget(AActor* Target)
{
	if (AIController)
		AIController->MoveToActor(Target, 20.f);
		CurrentTarget = Target;
}

void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

