// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputAction.h"
#include "InputActionValue.h"
#include "PlayerCharacter.generated.h"

class UStaticMeshComponent;
class USpringArmComponent;
class UCameraComponent;
class UInputAction;
class UCurveFloat;
class UCameraShakeSourceComponent;
class UTimelineComponent;
class ABullet;

UCLASS()
class PROJECTMAZE_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	APlayerCharacter();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Camera") TObjectPtr<USpringArmComponent> CameraBoom;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Camera") TObjectPtr<UCameraComponent> MainCamera;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera") TObjectPtr<UCameraShakeSourceComponent> CameraShakeSourceComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Timeline") TObjectPtr<UCurveFloat> CameraZoomInCurve;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Timeline") TObjectPtr<UCurveFloat> CameraZoomOutCurve;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Timeline") TObjectPtr<UTimelineComponent> TimelineComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement") TObjectPtr<UCurveFloat> MovementSpeedCurve;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon") TObjectPtr<USkeletalMeshComponent> WeaponSkeletalMesh;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon") TObjectPtr<USceneComponent> LaserPoint;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon") float LaserTraceDistance;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Bullet") TSubclassOf<ABullet> Bullet;


protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EnhancedInput") TObjectPtr<UInputMappingContext> InputMappingContext;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "EnhancedInput") TObjectPtr<UInputAction> InputMoveForward;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "EnhancedInput") TObjectPtr<UInputAction> InputMoveBackward;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "EnhancedInput") TObjectPtr<UInputAction> InputMoveRight;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "EnhancedInput") TObjectPtr<UInputAction> InputMoveLeft;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "EnhancedInput") TObjectPtr<UInputAction> InputLookAround;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "EnhancedInput") TObjectPtr<UInputAction> InputLookUp;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "EnhancedInput") TObjectPtr<UInputAction> InputRun;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "EnhancedInput") TObjectPtr<UInputAction> InputAim;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "EnhancedInput") TObjectPtr<UInputAction> InputFire;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "EnhancedInput") TObjectPtr<UInputAction> InputChangeCameraAngle;
	
	UPROPERTY(EditAnywhere, Category = "EnhancedInput") float LookUpSensitivity = 1.f;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "EnhancedInput") bool MoveInputPressed = false;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "EnhancedInput") bool MoveInputReleased = false;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "EnhancedInput") bool MoveBackInputPressed = false;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "EnhancedInput") bool MoveBackInputReleased = false;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "EnhancedInput") bool MoveRightInputPressed = false;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "EnhancedInput") bool MoveRightInputReleased = false;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "EnhancedInput") bool MoveLeftInputPressed = false;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "EnhancedInput") bool MoveLeftInputReleased = false;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "EnhancedInput") bool RunInputPressed = false;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "EnhancedInput") bool RunInputReleased = false;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "EnhancedInput") bool AimInputPressed = false;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "EnhancedInput") bool AimInputReleased = false;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "EnhancedInput") bool FireInputPressed = false;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "EnhancedInput") bool IsRunning = false;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "EnhancedInput") bool IsAiming = false;
	
	void StartMoveForward(const FInputActionInstance& Value);
	void StopMoveForward();
	void StartMoveBackward(const FInputActionInstance& Value);
	void StopMoveBackward();
	void StrafeRight(const FInputActionInstance& Value);
	void StrafeLeft(const FInputActionInstance& Value);
	void StopStrafeRight();
	void StopStrafeLeft();
	void Turn(const FInputActionValue& Value);
	void LookUp(const FInputActionValue& Value);
	void Fire(const FInputActionInstance& Value);
	void StopFire();
	void ChangeCameraAngle();
	void StartAim();
	void StopAim();
	UFUNCTION() void TimelineCameraZoomIn(const float Output) const;
	UFUNCTION() void TimelineCameraZoomOut(const float Output) const;

private:
	UPROPERTY(EditAnywhere, Category = "Camera") float MinPitch;
	UPROPERTY(EditAnywhere, Category = "Camera") float MaxPitch;
	UPROPERTY(EditAnywhere, Category = "Camera") float CurrentCameraBoomLength;
	UPROPERTY(EditAnywhere, Category = "Camera") float DefaultCameraBoomLength = 130.f;
	UPROPERTY(EditAnywhere, Category = "Camera") float WalkingCameraBoomLength = 160.f;
	UPROPERTY(EditAnywhere, Category = "Camera") float RunningCameraBoomLength = 200.f;
	UPROPERTY(EditAnywhere, Category = "Camera") float CameraBoomInterpolationSpeed = 1.f;

	bool bHitSomething;
	FHitResult HitResult;
	FCollisionQueryParams LaserTraceParams;
};
