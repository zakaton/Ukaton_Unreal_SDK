// Copyright (c) 2023 Zack Qattan

#include "UkatonSensorData.h"
#include "ByteParser.h"

#define ENABLE_UKATON_SENSOR_DATA_LOGGING true

#if ENABLE_UKATON_SENSOR_DATA_LOGGING
#define UKATON_SENSOR_DATA_LOG_VERBOSITY Log
#else
#define UKATON_SENSOR_DATA_LOG_VERBOSITY None
#endif

DECLARE_LOG_CATEGORY_EXTERN(UkatonSensorData, Log, All);

void FUkatonSensorData::SetDeviceType(EUkatonDeviceType NewDeviceType)
{
    DeviceType = NewDeviceType;

    MotionData.SetDeviceType(NewDeviceType);
    PressureData.SetDeviceType(NewDeviceType);
}

uint8 FUkatonSensorData::ParseData(const TArray<uint8> &Data, uint8 Offset)
{
    // FILL
    return Offset;
}