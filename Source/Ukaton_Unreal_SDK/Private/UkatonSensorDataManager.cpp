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

uint8 FUkatonSensorDataManager::ParseSensorData(const TArray<uint8> &Data, uint8 Offset)
{
    LastTimeReceivedSensorData = FGenericPlatformTime::Seconds();

    uint16 RawTimestamp = ByteParser::GetUint16(Data, Offset);
    Offset += 2;
    if (RawTimestamp < LastRawTimestamp)
    {
        TimestampOffset += UINT16_MAX + 1;
    }
    Timestamp = RawTimestamp + Offset;
    LastRawTimestamp = RawTimestamp;

    while (Offset < Data.Num())
    {
        auto SensorType = (EUkatonSensorType)Data[Offset++];
        auto SensorDataSize = Data[Offset++];
        auto FinalOffset = Offset + SensorDataSize;
        switch (SensorType)
        {
        case EUkatonSensorType::MOTION:
            Offset = MotionData.ParseData(Data, Offset, FinalOffset);
            break;
        case EUkatonSensorType::PRESSURE:
            Offset = PressureData.ParseData(Data, Offset, FinalOffset);
            break;
        default:
            UE_LOGFMT(UkatonSensorData, Error, "Uncaught handler for SensorType: {0}", static_cast<uint8>(SensorType));
            break;
        }
    }

    return Offset;
}