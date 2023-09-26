// Copyright (c) 2023 Zack Qattan

#include "UkatonHapticsManager.h"
#include "UkatonVibrationType.h"
#include "Logging/StructuredLog.h"

DEFINE_LOG_CATEGORY(LogUkatonHapticsManager);

void FUkatonHapticsManager::SerializeWaveforms(const TArray<EUkatonVibrationWaveformType> &Waveforms)
{
    SerializedVibration.Reset();
    SerializedVibration.Add(static_cast<uint8>(EUkatonVibrationType::WAVEFORM));
    for (uint8 i = 0; i < Waveforms.Num(); i++)
    {
        SerializedVibration.Add(static_cast<uint8>(Waveforms[i]));
    }
}
void FUkatonHapticsManager::SerializeSequence(const TArray<FUkatonVibrationSequenceSegment> &Sequence)
{
    SerializedVibration.Reset();
    SerializedVibration.Add(static_cast<uint8>(EUkatonVibrationType::SEQUENCE));
    for (uint8 i = 0; i < Sequence.Num(); i++)
    {
        auto Segment = Sequence[i];
        SerializedVibration.Add(static_cast<uint8>(Segment.Amplitude * 126));
        SerializedVibration.Add(static_cast<uint8>(Segment.Delay / 10));
    }
}