// Copyright (c) 2023 Zack Qattan

#include "UkatonSensorDataManager.h"
#include "ByteParser.h"
#include "Logging/StructuredLog.h"
#include "GenericPlatform/GenericPlatformTime.h"
#include <limits>

DECLARE_LOG_CATEGORY_EXTERN(UkatonSensorData, Log, All);

void FUkatonSensorDataManager::SetDeviceType(EUkatonDeviceType NewDeviceType)
{
    DeviceType = NewDeviceType;

    MotionData.SetDeviceType(NewDeviceType);
    PressureData.SetDeviceType(NewDeviceType);
}

uint8 FUkatonSensorDataManager::ParseData(const TArray<uint8> &Data, uint8 Offset)
{
    LastTimeReceivedData = FGenericPlatformTime::Seconds();

    uint16 RawTimestamp = ByteParser::GetUint16(Data, Offset);
    Offset += 2;
    if (RawTimestamp < LastRawDataTimestamp)
    {
        DataTimestampOffset += UINT16_MAX + 1;
    }
    Timestamp = RawTimestamp + Offset;
    LastRawDataTimestamp = RawTimestamp;

    // FILL

    return Offset;
}