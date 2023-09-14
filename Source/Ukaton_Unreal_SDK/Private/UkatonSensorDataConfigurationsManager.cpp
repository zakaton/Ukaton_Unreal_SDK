// Copyright (c) 2023 Zack Qattan

#include "UkatonSensorDataConfigurationsManager.h"
#include "ByteParser.h"
#include "Logging/StructuredLog.h"

DECLARE_LOG_CATEGORY_EXTERN(UkatonSensorDataConfigurationsManager, Log, All);

void FUkatonSensorDataConfigurationsManager::SetDeviceType(EUkatonDeviceType NewDeviceType)
{
    DeviceType = NewDeviceType;
}

const TArray<uint8> &FUkatonSensorDataConfigurationsManager::SerializeConfigurations() const
{
    // SerializedConfigurations.Reset();

    for (auto &MotionDataRate : MotionDataRates)
    {
        // FILL
    }

    if (DeviceType != EUkatonDeviceType::MOTION_MODULE)
    {
        for (auto &PressureDataRate : MotionDataRates)
        {
            // FILL
        }
    }
    return SerializedConfigurations;
}