// Copyright (c) 2023 Zack Qattan

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EUkatonMissionSide : uint8
{
	LEFT UMETA(DisplayName = "Left Insole"),
	RIGHT UMETA(DisplayName = "Right Insole"),
	COUNT UMETA(Hidden)
};