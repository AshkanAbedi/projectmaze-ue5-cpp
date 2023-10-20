// Fill out your copyright notice in the Description page of Project Settings.


#include "BulletSpawner.h"
#include "Bullet.h"

ABulletSpawner::ABulletSpawner()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ABulletSpawner::FireBullet(const FVector& StartingLocation, const FRotator& StartingRotation) const
{
	if (Bullet == nullptr)
		return;
	const ABullet* NewBullet = GetWorld()->SpawnActor<ABullet>(Bullet, this->GetActorLocation(), this->GetActorRotation());
	 if (NewBullet && GEngine)
	 	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Bullet Spawned"));
}

void ABulletSpawner::BeginPlay()
{
	Super::BeginPlay();
}



