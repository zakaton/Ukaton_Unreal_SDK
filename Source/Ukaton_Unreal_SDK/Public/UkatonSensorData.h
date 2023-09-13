// Copyright (c) 2023 Zack Qattan

#pragma once

#include "CoreMinimal.h"
#include "Math/Vector2D.h"
#include "UkatonDeviceType.h"
#include "UkatonMotionData.h"
#include "UkatonPressureData.h"
#include "UkatonSensorData.generated.h"

USTRUCT(BlueprintType)
struct FUkatonSensorData
{
    GENERATED_BODY()

    void SetDeviceType(EUkatonDeviceType NewDeviceType);

    UPROPERTY(BlueprintReadOnly, Category = "Ukaton Sensor Data")
    FUkatonMotionData MotionData;

    UPROPERTY(BlueprintReadOnly, Category = "Ukaton Sensor Data")
    FUkatonPressureData PressureData;

    uint8 ParseData(const TArray<uint8> &Data, uint8 Offset);

private:
    EUkatonDeviceType DeviceType;
};
