// Copyright (c) 2023 Zack Qattan

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EUkatonSensorDataRate : uint8
{
	Value0 UMETA(DisplayName = "0"),
	Value20 UMETA(DisplayName = "20"),
	Value40 UMETA(DisplayName = "40"),
	Value60 UMETA(DisplayName = "60"),
	Value80 UMETA(DisplayName = "80"),
	Value100 UMETA(DisplayName = "100")
};