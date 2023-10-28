// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectMaze/Public/PlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnhancedInput/Public/EnhancedInputComponent.h"
#include "Curves/CurveFloat.h"
#include "Camera/CameraShakeSourceComponent.h"
#include "Components/TimelineComponent.h"
#include "Bullet.h"

APlayerCharacter::APlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("Camera Boom"));
	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->TargetArmLength = 130.f;
	CameraBoom->SetRelativeLocation(FVector (0, 40, 80));

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
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	CurrentCameraBoomLength = DefaultCameraBoomLength;
	GetWorld()->GetTimerManager().SetTimer(LaserTimerHandle, this, &APlayerCharacter::LaserTimer, 0.016f, true);
	
	if (IsValid(WeaponSkeletalMesh) && IsValid(LaserPoint))
		WeaponSkeletalMesh->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("WeaponSocket"));
		LaserPoint->AttachToComponent(WeaponSkeletalMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("LaserPointSocket"));
}

void APlayerCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	GetWorld()->GetTimerManager().ClearTimer(AimingTimerHandle);
	GetWorld()->GetTimerManager().ClearTimer(LaserTimerHandle);
}

void APlayerCharacter::StartMoveForward(const FInputActionInstance& Value)
{
	if ((Controller) && (IsAiming))
	{
		AddMovementInput(FVector(GetActorForwardVector()), MovementSpeedCurve->GetFloatValue(Value.GetTriggeredTime()));
		GetCharacterMovement()->MaxWalkSpeed = 200.f;
	}
	if ((Controller) && (!IsAiming) && (!IsRunning))
	{
		AddMovementInput(FVector(GetActorForwardVector()), MovementSpeedCurve->GetFloatValue(Value.GetTriggeredTime()));
		GetCharacterMovement()->MaxWalkSpeed = 200.f;
		CurrentCameraBoomLength = WalkingCameraBoomLength;
	}
	else if ((Controller) && (!IsAiming) && (IsRunning))
	{
		AddMovementInput(FVector(GetActorForwardVector()), MovementSpeedCurve->GetFloatValue(Value.GetTriggeredTime()));
		GetCharacterMovement()->MaxWalkSpeed = 500.f;
		CurrentCameraBoomLength = RunningCameraBoomLength;
	}
}

void APlayerCharacter::StopMoveForward()
{
	CurrentCameraBoomLength = DefaultCameraBoomLength;
}

void APlayerCharacter::StartMoveBackward(const FInputActionInstance& Value)
{
	if ((Controller) && (IsAiming))
	{
		AddMovementInput(FVector(GetActorForwardVector() * -1), MovementSpeedCurve->GetFloatValue(Value.GetTriggeredTime()));
		GetCharacterMovement()->MaxWalkSpeed = 200.f;
	}
	if ((Controller) && (!IsAiming) && (!IsRunning))
	{
		AddMovementInput(FVector(GetActorForwardVector() * -1), MovementSpeedCurve->GetFloatValue(Value.GetTriggeredTime()));
		GetCharacterMovement()->MaxWalkSpeed = 200.f;
	}
	else if ((Controller) && (!IsAiming) && (IsRunning))
	{
		AddMovementInput(FVector(GetActorForwardVector() * -1), MovementSpeedCurve->GetFloatValue(Value.GetTriggeredTime()));
		GetCharacterMovement()->MaxWalkSpeed = 500.f;
	}
}

void APlayerCharacter::StartMoveRight(const FInputActionInstance& Value)
{
	if ((Controller) && (IsAiming))
	{
		AddMovementInput(FVector(GetActorRightVector()), MovementSpeedCurve->GetFloatValue(Value.GetTriggeredTime()));
		GetCharacterMovement()->MaxWalkSpeed = 200.f;
	}
	if ((Controller) && (!IsAiming) && (!IsRunning))
	{
		AddMovementInput(FVector(GetActorRightVector()), MovementSpeedCurve->GetFloatValue(Value.GetTriggeredTime()));
		GetCharacterMovement()->MaxWalkSpeed = 200.f;
	}
	else if ((Controller) && (!IsAiming) && (IsRunning))
	{
		AddMovementInput(FVector(GetActorRightVector()), MovementSpeedCurve->GetFloatValue(Value.GetTriggeredTime()));
		GetCharacterMovement()->MaxWalkSpeed = 500.f;
	}
}

void APlayerCharacter::StartMoveLeft(const FInputActionInstance& Value)
{
	if ((Controller) && (IsAiming))
	{
		AddMovementInput(FVector(GetActorRightVector() * -1.f), MovementSpeedCurve->GetFloatValue(Value.GetTriggeredTime()));
		GetCharacterMovement()->MaxWalkSpeed = 200.f;
	}
	if ((Controller) && (!IsAiming) && (!IsRunning))
	{
		AddMovementInput(FVector(GetActorRightVector() * -1.f), MovementSpeedCurve->GetFloatValue(Value.GetTriggeredTime()));
		GetCharacterMovement()->MaxWalkSpeed = 200.f;
	}
	else if ((Controller) && (!IsAiming) && (IsRunning))
	{
		AddMovementInput(FVector(GetActorRightVector() * -1.f), MovementSpeedCurve->GetFloatValue(Value.GetTriggeredTime()));
		GetCharacterMovement()->MaxWalkSpeed = 500.f;
	}
}

void APlayerCharacter::StartRun()
{
	IsRunning = true;
}

void APlayerCharacter::StopRun()
{
	IsRunning = false;
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
	if (Bullet == nullptr)
		return;
	if (IsAiming == true)
	{
		GetWorld()->SpawnActor<ABullet>(Bullet, LaserPoint->GetComponentLocation(), LaserPoint->GetComponentRotation());
		IsFiring = true;
	}
	else
	{
		return;
	}	
}

void APlayerCharacter::StopFire()
{
	IsFiring = false;
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
	GetWorld()->GetTimerManager().SetTimer(AimingTimerHandle, this, &APlayerCharacter::AimingTimer, 0.016f, true);

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
	GetWorld()->GetTimerManager().ClearTimer(AimingTimerHandle);
	
	FOnTimelineFloat TimelineTickDelegate;
	TimelineTickDelegate.BindUFunction(this, "TimelineCameraZoomOut");
	TimelineComponent->AddInterpFloat(CameraZoomOutCurve, TimelineTickDelegate);
	TimelineComponent->SetTimelineLength(ETimelineLengthMode::TL_TimelineLength);
	TimelineComponent->PlayFromStart();
}

void APlayerCharacter::AimingTimer()
{
	bHitSomething = GetWorld()->LineTraceSingleByChannel(HitResult, LaserPoint->GetComponentLocation(), LaserPoint->GetComponentLocation() + (LaserPoint->GetForwardVector() * LaserTraceDistance), ECC_Visibility, LaserTraceParams);
	DrawDebugLine(GetWorld(), LaserPoint->GetComponentLocation(), LaserPoint->GetComponentLocation() + (LaserPoint->GetForwardVector() * LaserTraceDistance), FColor::Red, false, -1.f, 0, 1.f);
}

void APlayerCharacter::LaserTimer()
{
	LaserPoint->SetWorldRotation(CameraBoom->GetTargetRotation());
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
}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	EnhancedInputComponent->BindAction(InputMoveForward, ETriggerEvent::Triggered, this, &APlayerCharacter::StartMoveForward);
	EnhancedInputComponent->BindAction(InputMoveForward, ETriggerEvent::Completed, this, &APlayerCharacter::StopMoveForward);
	EnhancedInputComponent->BindAction(InputMoveBackward, ETriggerEvent::Triggered, this, &APlayerCharacter::StartMoveBackward);
	EnhancedInputComponent->BindAction(InputMoveRight, ETriggerEvent::Triggered, this, &APlayerCharacter::StartMoveRight);
	EnhancedInputComponent->BindAction(InputMoveLeft, ETriggerEvent::Triggered, this, &APlayerCharacter::StartMoveLeft);
	EnhancedInputComponent->BindAction(InputRun, ETriggerEvent::Started, this, &APlayerCharacter::StartRun);
	EnhancedInputComponent->BindAction(InputRun, ETriggerEvent::Completed, this, &APlayerCharacter::StopRun);
	EnhancedInputComponent->BindAction(InputLookAround, ETriggerEvent::Triggered, this, &APlayerCharacter::Turn);
	EnhancedInputComponent->BindAction(InputLookUp, ETriggerEvent::Triggered, this, &APlayerCharacter::LookUp);
	EnhancedInputComponent->BindAction(InputAim, ETriggerEvent::Started, this, &APlayerCharacter::StartAim);
	EnhancedInputComponent->BindAction(InputAim, ETriggerEvent::Completed, this, &APlayerCharacter::StopAim);
	EnhancedInputComponent->BindAction(InputFire, ETriggerEvent::Started, this, &APlayerCharacter::Fire);
	EnhancedInputComponent->BindAction(InputFire, ETriggerEvent::Completed, this, &APlayerCharacter::StopFire);
	EnhancedInputComponent->BindAction(InputChangeCameraAngle, ETriggerEvent::Started, this, &APlayerCharacter::ChangeCameraAngle);
	
}

