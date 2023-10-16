// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"
#include "Components/SkeletalMeshComponent.h"


AWeapon::AWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Weapon Mesh"));
	WeaponMesh->SetupAttachment(GetRootComponent());

}


void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}


void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

