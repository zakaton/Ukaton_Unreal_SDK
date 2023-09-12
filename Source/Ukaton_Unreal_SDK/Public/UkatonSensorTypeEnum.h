// Copyright (c) 2023 Zack Qattan

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EUkatonSensorTypeEnum : uint8
{
	MOTION UMETA(DisplayName = "Motion Sensor"),
	PRESSURE UMETA(DisplayName = "Pressure Sensor"),
	COUNT UMETA(Hidden)
};