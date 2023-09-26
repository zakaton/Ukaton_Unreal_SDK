// Copyright (c) 2023 Zack Qattan

#pragma once

#include "CoreMinimal.h"
#include "UkatonVibrationSequenceSegment.generated.h"

USTRUCT(BlueprintType)
struct FUkatonVibrationSequenceSegment
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, DisplayName = "Amplitude [0, 126]", meta = (ClampMin = "0", ClampMax = "126"))
    uint8 Amplitude;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, DisplayName = "Delay (s)")
    int Delay;

    FUkatonVibrationSequenceSegment(uint8 InAmplitude = 126, int InDelay = 100)
        : Amplitude(InAmplitude), Delay(InDelay)
    {
    }
};
