// Copyright (c) 2023 Zack Qattan

#pragma once

#include "CoreMinimal.h"
#include "UkatonVibrationType.h"
#include "UkatonVibrationWaveformType.h"
#include "UkatonHapticsManager.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogUkatonHapticsManager, Log, All);

USTRUCT(BlueprintType)
struct FUkatonHapticsManager
{
    GENERATED_BODY()

public:
    TArray<uint8> SerializedVibration;

    void SerializeWaveform(const TArray<EUkatonVibrationWaveformType> &Waveforms);
    void SerializeSequence();
};
