// Copyright (c) 2023 Zack Qattan

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
	EULER UMETA(Hidden, DisplayName = "Euler"),
	COUNT UMETA(Hidden)
};