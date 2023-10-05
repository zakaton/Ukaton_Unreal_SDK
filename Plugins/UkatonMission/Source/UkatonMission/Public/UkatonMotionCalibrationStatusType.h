// Copyright (c) 2023 Zack Qattan

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EUkatonMotionCalibrationStatusType: uint8
{
	UNRELIABLE UMETA(DisplayName = "Unreliable"),
	ACCURACY_LOW UMETA(DisplayName = "Accuracy Low"),
	ACCURACY_MEDIUM UMETA(DisplayName = "Accuracy Medium"),
	ACCURACY_HIGH UMETA(DisplayName = "Accuracy High"),
	COUNT UMETA(Hidden)
};