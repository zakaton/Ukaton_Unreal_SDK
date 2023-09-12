// // Copyright (c) 2023 Zack Qattan

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EUkatonDeviceType : uint8
{
	MOTION_MODULE UMETA(DisplayName = "Motion Module"),
	LEFT_INSOLE UMETA(DisplayName = "Left Insole"),
	RIGHT_INSOLE UMETA(DisplayName = "Right Insole"),
	COUNT UMETA(Hidden)
};