// Copyright (c) 2023 Zack Qattan

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EUkatonMotionCalibrationType : uint8
{
	ACCELEROMETER UMETA(DisplayName = "Accelerometer"),
	GYROSCOPE UMETA(DisplayName = "Gyroscope"),
	MAGNETOMETER UMETA(DisplayName = "Magnetometer"),
	QUATERNION UMETA(DisplayName = "Quaternion"),
	COUNT UMETA(Hidden)
};