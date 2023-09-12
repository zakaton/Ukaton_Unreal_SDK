// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EUkatonMotionDataType : uint8
{
	ACCELERATION UMETA(DisplayName = "Acceleration"),
	GRAVITY UMETA(DisplayName = "Gravity"),
	LINEAR_ACCELERATION UMETA(DisplayName = "Linear Acceleration"),
	ROTATION_RATE UMETA(DisplayName = "Rotation Rate"),
	MAGNETOMETER UMETA(DisplayName = "Magnetometer"),
	QUATERNION UMETA(DisplayName = "Quaternion"),
	EULER UMETA(DisplayName = "Euler"),
	COUNT UMETA(Hidden)
};