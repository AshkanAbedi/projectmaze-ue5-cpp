// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Bullet.generated.h"

class UProjectileMovementComponent;
class UBoxComponent;
class UParticleSystem;

UCLASS()
class PROJECTMAZE_API ABullet : public AActor
{
	GENERATED_BODY()
	
public:	
	ABullet();
	virtual void Tick(float DeltaTime) override;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Main") TObjectPtr<UBoxComponent> BulletCollision;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Main") TObjectPtr<UStaticMeshComponent> BulletMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Properties") float BulletSpeed = 1500.f;
	[[nodiscard]] float GetBulletSpeed() const{ return BulletSpeed;}

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Hit Effects") TObjectPtr<UParticleSystem> BulletHitParticle;


protected:
	virtual void BeginPlay() override;
	UFUNCTION() void OnBulletHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	UFUNCTION() void OnBulletOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true")) TObjectPtr<UProjectileMovementComponent> ProjectileMovementComponent;

};
