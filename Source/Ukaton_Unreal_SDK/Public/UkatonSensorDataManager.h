// Copyright (c) 2023 Zack Qattan

#pragma once

#include "CoreMinimal.h"
#include "UkatonDeviceType.h"
#include "UkatonMotionData.h"
#include "UkatonPressureData.h"
#include "UkatonSensorType.h"
#include "UkatonSensorDataManager.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogUkatonSensorDataManager, Log, All);

USTRUCT(BlueprintType)
struct FUkatonSensorDataManager
{
    GENERATED_BODY()

    void UpdateDeviceType(EUkatonDeviceType NewDeviceType);

    UPROPERTY(BlueprintReadOnly, Category = "Ukaton Sensor Data")
    FUkatonMotionData MotionData;

    UPROPERTY(BlueprintReadOnly, Category = "Ukaton Sensor Data")
    FUkatonPressureData PressureData;

    void ParseSensorData(const TArray<uint8> &Data, uint8 &Offset);

    uint32 Timestamp;
    double LastTimeReceivedSensorData;

private:
    EUkatonDeviceType DeviceType;

    uint16 LastRawTimestamp;
    uint32 TimestampOffset;
};
