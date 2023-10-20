// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectMaze/Public/PlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnhancedInput/Public/EnhancedInputComponent.h"
#include "Curves/CurveFloat.h"
#include "Camera/CameraShakeSourceComponent.h"
#include "Components/TimelineComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/KismetMathLibrary.h"

APlayerCharacter::APlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("Camera Boom"));
	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->TargetArmLength = 130.f;
	CameraBoom->SetRelativeLocation(FVector (0, 40, 80));
	CurrentCameraBoomLength = DefaultCameraBoomLength;

	MainCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Main Camera"));
	MainCamera->SetupAttachment(CameraBoom);

	TimelineComponent = CreateDefaultSubobject<UTimelineComponent>(TEXT("Timeline Component"));
	
	GetCharacterMovement()->MaxWalkSpeed = 500;
	
	CameraShakeSourceComponent = CreateDefaultSubobject<UCameraShakeSourceComponent>(TEXT("Camera Shake Source"));
	CameraShakeSourceComponent->SetupAttachment(CameraBoom);
	CameraShakeSourceComponent->SetAutoActivate(false);
	
	WeaponSkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Weapon Skeletal Mesh"));
	WeaponSkeletalMesh->SetupAttachment(GetMesh());
	LaserPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Laser Point"));
	LaserPoint->SetupAttachment(WeaponSkeletalMesh);
	BulletSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Bullet Spawn Point"));
	BulletSpawnPoint->SetupAttachment(WeaponSkeletalMesh);
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	if (IsValid(WeaponSkeletalMesh) && IsValid(LaserPoint) && IsValid (BulletSpawnPoint))
		WeaponSkeletalMesh->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("WeaponSocket"));
		LaserPoint->AttachToComponent(WeaponSkeletalMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("LaserPointSocket"));
		BulletSpawnPoint->AttachToComponent(WeaponSkeletalMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale,  FName("BulletSpawnPointSocket"));
}

void APlayerCharacter::StartMoveForward(const FInputActionInstance& Value)
{
	
	if ((Controller) && (IsAiming)){
		AddMovementInput(FVector(GetActorForwardVector()), 0.4);
		MoveInputPressed = true;
		MoveInputReleased = false;
	}
	else if ((Controller) && (!IsAiming)) {
		AddMovementInput(FVector(GetActorForwardVector()), MovementSpeedCurve->GetFloatValue(Value.GetTriggeredTime()));
		if (UKismetMathLibrary::VSizeXY(GetCharacterMovement()->Velocity) > 250.f)
			CurrentCameraBoomLength = RunningCameraBoomLength;
		MoveInputPressed = true;
		MoveInputReleased = false;
	}
}

void APlayerCharacter::StopMoveForward()
{
	MoveInputReleased = true;
	MoveInputPressed = false;
	CurrentCameraBoomLength = DefaultCameraBoomLength;
}

void APlayerCharacter::StartMoveBackward(const FInputActionInstance& Value)
{
	
	if ((Controller) && (IsAiming))
	{
		AddMovementInput(FVector(GetActorForwardVector() * -1), 0.4);
		MoveBackInputPressed = true;
		MoveBackInputReleased = false;
	}
}

void APlayerCharacter::StopMoveBackward()
{
	MoveBackInputPressed = false;
	MoveBackInputReleased = true;
}

void APlayerCharacter::StrafeRight(const FInputActionInstance& Value)
{
	
	if ((Controller) && (IsAiming))
	{
		StrafeRightInputPressed = true;
		StrafeRightInputReleased = false;
		AddMovementInput(FVector(GetActorRightVector()), 0.3);
	}
}

void APlayerCharacter::StrafeLeft(const FInputActionInstance& Value)
{
	
	if ((Controller) && (IsAiming))
	{
		StrafeLeftInputPressed = true;
		StrafeLeftInputReleased = false;
		AddMovementInput(FVector(GetActorRightVector() * -1), 0.3);
	}
	
}

void APlayerCharacter::StopStrafeRight()
{
	StrafeRightInputPressed = false;
	StrafeRightInputReleased = true;
}

void APlayerCharacter::StopStrafeLeft()
{
	StrafeLeftInputPressed = false;
	StrafeLeftInputReleased = true;
}

void APlayerCharacter::Turn(const FInputActionValue& Value)
{
	if ((Controller))
		AddControllerYawInput(Value.GetMagnitude());
}

// ReSharper disable once CppMemberFunctionMayBeConst
void APlayerCharacter::LookUp(const FInputActionValue& Value)
{
	if ((Controller) && Value.GetMagnitude() != 0 && CameraBoom->GetTargetRotation().Pitch + Value.GetMagnitude() < MaxPitch && CameraBoom->GetTargetRotation().Pitch + Value.GetMagnitude() > MinPitch)
		CameraBoom->AddLocalRotation(FRotator(Value.GetMagnitude(),0,0));
}

void APlayerCharacter::Fire(const FInputActionInstance& Value)
{
	FireInputPressed = true;
	
}

void APlayerCharacter::StopFire()
{
	FireInputPressed = false;
}

// ReSharper disable once CppMemberFunctionMayBeConst
void APlayerCharacter::ChangeCameraAngle()
{
	const FVector CBCurrentLocation = CameraBoom->GetRelativeLocation();
	const float MirrorRes = CBCurrentLocation.Y * -1;
	CameraBoom->SetRelativeLocation(FVector(CBCurrentLocation.X, MirrorRes, CBCurrentLocation.Z));
}

void APlayerCharacter::StartAim()
{
	IsAiming = true;
	AimInputPressed = true;
	AimInputReleased = false;

	if (!CameraZoomInCurve || !CameraZoomOutCurve)
	{
		UE_LOG(LogTemp, Error, TEXT("A Camera Curve is not valid!"));
		return;
	}
	
	FOnTimelineFloat TimelineTickDelegate;
	TimelineTickDelegate.BindUFunction(this, "TimelineCameraZoomIn");
	TimelineComponent->AddInterpFloat(CameraZoomInCurve, TimelineTickDelegate);
	TimelineComponent->SetTimelineLength(ETimelineLengthMode::TL_TimelineLength);
	TimelineComponent->Activate();
	TimelineComponent->PlayFromStart();
}

void APlayerCharacter::StopAim()
{
	IsAiming = false;
	AimInputPressed = false;
	AimInputReleased = true;
	FOnTimelineFloat TimelineTickDelegate;
	TimelineTickDelegate.BindUFunction(this, "TimelineCameraZoomOut");
	TimelineComponent->AddInterpFloat(CameraZoomOutCurve, TimelineTickDelegate);
	TimelineComponent->SetTimelineLength(ETimelineLengthMode::TL_TimelineLength);
	TimelineComponent->PlayFromStart();
}

void APlayerCharacter::TimelineCameraZoomIn(const float Output) const
{
	MainCamera->SetFieldOfView(Output);
}

void APlayerCharacter::TimelineCameraZoomOut(const float Output) const
{
	MainCamera->SetFieldOfView(Output);
}

void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	CameraBoom->TargetArmLength = FMath::FInterpTo(CameraBoom->TargetArmLength, CurrentCameraBoomLength, DeltaTime, CameraBoomInterpolationSpeed);

	if (IsAiming)
		bHitSomething = GetWorld()->LineTraceSingleByChannel(HitResult, LaserPoint->GetComponentLocation(), LaserPoint->GetComponentLocation() + (LaserPoint->GetForwardVector() * LaserTraceDistance), ECC_Visibility, LaserTraceParams);
		DrawDebugLine(GetWorld(), LaserPoint->GetComponentLocation(), LaserPoint->GetComponentLocation() + (LaserPoint->GetForwardVector() * LaserTraceDistance), FColor::Red, false, -1.f, 0, 1.f);
		LaserPoint->SetWorldRotation(CameraBoom->GetTargetRotation());
}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	EnhancedInputComponent->BindAction(InputMoveForward, ETriggerEvent::Triggered, this, &APlayerCharacter::StartMoveForward);
	EnhancedInputComponent->BindAction(InputMoveForward, ETriggerEvent::Completed, this, &APlayerCharacter::StopMoveForward);
	EnhancedInputComponent->BindAction(InputMoveBackward, ETriggerEvent::Triggered, this, &APlayerCharacter::StartMoveBackward);
	EnhancedInputComponent->BindAction(InputMoveBackward, ETriggerEvent::Completed, this, &APlayerCharacter::StopMoveBackward);
	EnhancedInputComponent->BindAction(InputStrafeRight, ETriggerEvent::Triggered, this, &APlayerCharacter::StrafeRight);
	EnhancedInputComponent->BindAction(InputStrafeLeft, ETriggerEvent::Triggered, this, &APlayerCharacter::StrafeLeft);
	EnhancedInputComponent->BindAction(InputStrafeRight, ETriggerEvent::Completed, this, &APlayerCharacter::StopStrafeRight);
	EnhancedInputComponent->BindAction(InputStrafeLeft, ETriggerEvent::Completed, this, &APlayerCharacter::StopStrafeLeft);
	EnhancedInputComponent->BindAction(InputLookAround, ETriggerEvent::Triggered, this, &APlayerCharacter::Turn);
	EnhancedInputComponent->BindAction(InputLookUp, ETriggerEvent::Triggered, this, &APlayerCharacter::LookUp);
	EnhancedInputComponent->BindAction(InputAim, ETriggerEvent::Started, this, &APlayerCharacter::StartAim);
	EnhancedInputComponent->BindAction(InputAim, ETriggerEvent::Completed, this, &APlayerCharacter::StopAim);
	EnhancedInputComponent->BindAction(InputFire, ETriggerEvent::Triggered, this, &APlayerCharacter::Fire);
	EnhancedInputComponent->BindAction(InputFire, ETriggerEvent::Completed, this, &APlayerCharacter::StopFire);
	EnhancedInputComponent->BindAction(InputChangeCameraAngle, ETriggerEvent::Started, this, &APlayerCharacter::ChangeCameraAngle);

	
}

