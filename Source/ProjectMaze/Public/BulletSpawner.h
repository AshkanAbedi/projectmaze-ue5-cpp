// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BulletSpawner.generated.h"

class ABullet;

UCLASS()
class PROJECTMAZE_API ABulletSpawner : public AActor
{
	GENERATED_BODY()
	
// public:	
	ABulletSpawner();
// 	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly) TObjectPtr<USceneComponent> BulletSpawnerRoot;
// 	UPROPERTY(EditAnywhere, BlueprintReadOnly) TSubclassOf<class ABullet> Bullet;
// 	
// 	UFUNCTION() void FireBullet(const FVector& StartingLocation, const FRotator& StartingRotation) const;

protected:
	virtual void BeginPlay() override;

private:
	
};
