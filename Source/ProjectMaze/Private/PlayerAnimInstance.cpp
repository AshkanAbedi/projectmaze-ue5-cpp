// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerAnimInstance.h"
#include "KismetAnimationLibrary.h"
#include "GameFramework/SpringArmComponent.h"
#include "PlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

void UPlayerAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	PlayerCharacter = Cast<APlayerCharacter>(TryGetPawnOwner());
	if (IsValid(PlayerCharacter))
		PlayerMovementComponent = PlayerCharacter->GetCharacterMovement();
		// CameraBoom = PlayerCharacter->CameraBoom;
		// PlayerCurrentVelocity = PlayerCharacter->GetVelocity();
		// PlayerCurrentRotation = PlayerCharacter->GetActorRotation();
}

void UPlayerAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	if (IsValid(PlayerMovementComponent))
		GroundSpeed = UKismetMathLibrary::VSizeXY(PlayerMovementComponent->Velocity);
		// AimingPitch =  CameraBoom->GetTargetRotation().Pitch;
		// Direction = UKismetAnimationLibrary::CalculateDirection(PlayerCurrentVelocity, PlayerCurrentRotation);
}
