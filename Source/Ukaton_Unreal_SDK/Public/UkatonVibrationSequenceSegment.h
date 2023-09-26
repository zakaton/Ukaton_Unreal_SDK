// Copyright (c) 2023 Zack Qattan

#pragma once

#include "CoreMinimal.h"
#include "UkatonVibrationSequenceSegment.generated.h"

USTRUCT(BlueprintType)
struct FUkatonVibrationSequenceSegment
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, DisplayName = "Amplitude [0, 1]", meta = (ClampMin = "0", ClampMax = "1"))
    float Amplitude;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, DisplayName = "Delay (s)", meta = (ClampMin = "0", ClampMax = "2000"))
    int Delay;

    FUkatonVibrationSequenceSegment(float InAmplitude = 1, int InDelay = 100)
        : Amplitude(InAmplitude), Delay(InDelay)
    {
    }
};
