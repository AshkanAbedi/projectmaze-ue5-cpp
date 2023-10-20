// Fill out your copyright notice in the Description page of Project Settings.


#include "Bullet.h"
#include "Components/BoxComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystem.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ABullet::ABullet()
{
	PrimaryActorTick.bCanEverTick = true;
	BulletCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Bullet Collision"));
	this->SetRootComponent(BulletCollision);
	
	BulletMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Bullet Mesh"));
	BulletMesh->SetupAttachment(BulletCollision);

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement Component"));
	BulletHitParticle = CreateDefaultSubobject<UParticleSystem>(TEXT("Bullet Hit Particle"));
}

void ABullet::BeginPlay()
{
	Super::BeginPlay();
	SetLifeSpan(0.f);
	// BulletCollision->OnComponentBeginOverlap.AddDynamic(this, &ABullet::OnBulletOverlap);
	BulletCollision->OnComponentHit.AddDynamic(this, &ABullet::OnBulletHit);
	
}

// ReSharper disable once CppMemberFunctionMayBeStatic
// ReSharper disable once CppMemberFunctionMayBeConst
// ReSharper disable once CppParameterMayBeConstPtrOrRef
void ABullet::OnBulletHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	const FString OtherActorName = OtherActor->GetName();
	if ((GEngine) && (BulletHitParticle))
		GEngine->AddOnScreenDebugMessage(1, 20.f, FColor::Red, OtherActorName);
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), BulletHitParticle, Hit.Location, FRotator::ZeroRotator, true);
		Destroy();
}

// ReSharper disable once CppMemberFunctionMayBeStatic
// ReSharper disable once CppParameterMayBeConstPtrOrRef
void ABullet::OnBulletOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	const FString OtherActorName = OtherActor->GetName();
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(1, 20.f, FColor::Red, OtherActorName);
}

void ABullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


