// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PlayerAnimInstance.generated.h"

class APlayerCharacter;
class UCharacterMovementComponent;



UCLASS()
class PROJECTMAZE_API UPlayerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	
	UPROPERTY(BlueprintReadOnly) TObjectPtr<APlayerCharacter> PlayerCharacter;
	UPROPERTY(BlueprintReadOnly) TObjectPtr<UCharacterMovementComponent> PlayerMovementComponent;
	UPROPERTY(BlueprintReadWrite) double GroundSpeed;
	UPROPERTY(BlueprintReadWrite) double Direction;
	UPROPERTY(BlueprintReadWrite) double AimingPitch;

protected:

private:
	
};
