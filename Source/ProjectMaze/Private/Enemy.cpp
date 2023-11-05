// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AIController.h"
#include "PlayerCharacter.h"
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
	SelectRandomPatrolTarget();

	PawnSensingComponent->OnSeePawn.AddDynamic(this, &AEnemy::OnSeePawn);
}

void AEnemy::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	GetWorldTimerManager().ClearTimer(CheckDistanceTimerHandle);
	GetWorldTimerManager().ClearTimer(WaitTimerHandle);
	GetWorldTimerManager().ClearTimer(AlertTimerHandle);
}

// ReSharper disable once CppMemberFunctionMayBeConst
void AEnemy::OnSeePawn(APawn* Pawn)
{
	if (Pawn->ActorHasTag(FName("Player")) && EnemyState != EEnemyState::EES_Dead)
		EnemyState = EEnemyState::EES_Alert;
		GetCharacterMovement()->MaxWalkSpeed = 350.f;
		AIController->MoveToActor(Pawn, 10.f);
		GetWorldTimerManager().SetTimer(AlertTimerHandle, this, &AEnemy::IgnorePawn, 0.1f, true);
		GetWorldTimerManager().ClearTimer(CheckDistanceTimerHandle);
		GetWorldTimerManager().ClearTimer(WaitTimerHandle);
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("I SEE YOU!")));
}

void AEnemy::IgnorePawn()
{
	const double DistanceToPlayer = (GetActorLocation() - UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)->GetActorLocation()).Size();
	GEngine->AddOnScreenDebugMessage(1, 5.f, FColor::Red, FString::Printf(TEXT("Distance to Player: %f"), DistanceToPlayer));
	
	if (DistanceToPlayer > PawnSensingComponent->SightRadius && EnemyState == EEnemyState::EES_Alert)
		EnemyState = EEnemyState::EES_Patrolling;
		GetCharacterMovement()->MaxWalkSpeed = 110.f;
		GetWorldTimerManager().ClearTimer(AlertTimerHandle);
		SelectRandomPatrolTarget();
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("I LOST YOU!")));
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

