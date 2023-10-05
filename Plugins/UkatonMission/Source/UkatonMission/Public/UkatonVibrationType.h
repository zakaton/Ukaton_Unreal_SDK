// Copyright (c) 2023 Zack Qattan

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EUkatonVibrationType : uint8
{
	WAVEFORM UMETA(DisplayName = "Waveform"),
	SEQUENCE UMETA(DisplayName = "Sequence"),
	COUNT UMETA(Hidden)
};