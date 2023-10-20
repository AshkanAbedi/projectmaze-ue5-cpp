// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Bullet.generated.h"

class UProjectileMovementComponent;

UCLASS()
class PROJECTMAZE_API ABullet : public AActor
{
	GENERATED_BODY()
	
public:	
	ABullet();
	virtual void Tick(float DeltaTime) override;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Main") TObjectPtr<USceneComponent> BulletRoot;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Main") TObjectPtr<UStaticMeshComponent> BulletMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Properties") float BulletSpeed = 1500.f;
	[[nodiscard]] float GetBulletSpeed() const{ return BulletSpeed;}

	

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true")) TObjectPtr<UProjectileMovementComponent> ProjectileMovementComponent;
};
