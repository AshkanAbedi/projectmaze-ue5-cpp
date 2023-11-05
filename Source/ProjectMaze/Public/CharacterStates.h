// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EPlayerState : uint8
{
	EPS_Idle UMETA(DisplayName = "Idle"),
	EPS_Walking UMETA(DisplayName = "Walking"),
	EPS_Running UMETA(DisplayName = "Running"),
	EPS_Attacking UMETA(DisplayName = "Attacking"),
	EPS_Dead UMETA(DisplayName = "Dead")
};

UENUM(BlueprintType)
enum class EEnemyState : uint8
{
	EES_Idle UMETA(DisplayName = "Idle"),
	EES_Patrolling UMETA(DisplayName = "Patrolling"),
	EES_Chasing UMETA(DisplayName = "Chasing"),
	EES_RunningToPlayer UMETA(DisplayName = "runningToPlayer"),
	EES_Attacking UMETA(DisplayName = "Attacking"),
	EES_Hit UMETA(DisplayName = "Hit"),
	EES_Dead UMETA(DisplayName = "Dead")
};

class PROJECTMAZE_API CharacterStates
{
public:
	CharacterStates();
	~CharacterStates();
};
