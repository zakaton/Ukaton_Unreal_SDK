// Copyright (c) 2023 Zack Qattan

#include "UkatonSensorDataManager.h"
#include "ByteParser.h"
#include "Logging/StructuredLog.h"

DECLARE_LOG_CATEGORY_EXTERN(UkatonSensorData, Log, All);

void FUkatonSensorDataManager::SetDeviceType(EUkatonDeviceType NewDeviceType)
{
    DeviceType = NewDeviceType;

    MotionData.SetDeviceType(NewDeviceType);
    PressureData.SetDeviceType(NewDeviceType);
}

uint8 FUkatonSensorDataManager::ParseData(const TArray<uint8> &Data, uint8 Offset)
{
    // FILL
    return Offset;
}