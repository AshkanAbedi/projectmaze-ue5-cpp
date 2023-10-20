// Fill out your copyright notice in the Description page of Project Settings.


#include "Bullet.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
ABullet::ABullet()
{
	PrimaryActorTick.bCanEverTick = true;
	BulletMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Bullet Mesh"));
	BulletMesh->SetupAttachment(BulletRoot);

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement Component"));
	ProjectileMovementComponent->InitialSpeed = BulletSpeed;
}

void ABullet::BeginPlay()
{
	Super::BeginPlay();
	SetLifeSpan(5.f);
}

void ABullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

