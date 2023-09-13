// Copyright (c) 2023 Zack Qattan + ChatGPT

#pragma once

#include "CoreMinimal.h"

template <typename TEnum>
struct UEnumFlagManager
{
public:
    // Set a flag
    void SetFlag(TEnum Flag)
    {
        EnumFlags |= (1 << static_cast<int32>(Flag));
    }

    // Clear a flag
    void ClearFlag(TEnum Flag)
    {
        EnumFlags &= ~(1 << static_cast<int32>(Flag));
    }

    // Check if a flag is set
    bool IsFlagSet(TEnum Flag) const
    {
        return (EnumFlags & (1 << static_cast<int32>(Flag))) != 0;
    }

    // Reset all flags to their initial state
    void ResetFlags()
    {
        EnumFlags = 0;
    }

    // Iterate through set flags and perform an action on each one
    void IterateSetFlags(std::function<void(TEnum)> Action, bool ResetAfterIteration = false) const
    {
        int32 FlagToCheck = 1;
        int32 BitIndex = 0;
        while (FlagToCheck <= EnumFlags)
        {
            if ((EnumFlags & FlagToCheck) != (0))
            {
                Action(static_cast<TEnum>(BitIndex));
            }
            FlagToCheck = (FlagToCheck << 1);
            ++BitIndex;
        }

        if (ResetAfterIteration)
        {
            ResetFlags();
        }
    }

private:
    int32 EnumFlags = 0;
};