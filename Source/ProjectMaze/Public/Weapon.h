// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

class USkeletalMeshComponent;

UCLASS()
class PROJECTMAZE_API AWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	AWeapon();
	virtual void Tick(float DeltaTime) override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Weapon") TObjectPtr<USkeletalMeshComponent> WeaponMesh;

protected:
	virtual void BeginPlay() override;

private:
	
};
