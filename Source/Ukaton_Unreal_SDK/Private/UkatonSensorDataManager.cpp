// Copyright (c) 2023 Zack Qattan

#include "UkatonSensorDataManager.h"
#include "ByteParser.h"
#include "Logging/StructuredLog.h"
#include "GenericPlatform/GenericPlatformTime.h"
#include <limits>

DEFINE_LOG_CATEGORY(LogUkatonSensorDataManager);

void FUkatonSensorDataManager::UpdateDeviceType(EUkatonDeviceType NewDeviceType)
{
    DeviceType = NewDeviceType;

    MotionData.UpdateDeviceType(DeviceType);
    PressureData.UpdateDeviceType(DeviceType);
}

void FUkatonSensorDataManager::ParseSensorData(const TArray<uint8> &Data, uint8 &Offset)
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
            MotionData.ParseData(Data, Offset, FinalOffset);
            break;
        case EUkatonSensorType::PRESSURE:
            PressureData.ParseData(Data, Offset, FinalOffset);
            break;
        default:
            UE_LOGFMT(LogUkatonSensorDataManager, Error, "Uncaught handler for SensorType: {0}", static_cast<uint8>(SensorType));
            break;
        }
    }
}