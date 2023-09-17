// Copyright (c) 2023 Zack Qattan

#pragma once

#include "CoreMinimal.h"
#include "UkatonDeviceType.h"
#include "UkatonSensorType.h"
#include "UkatonMotionDataType.h"
#include "UkatonPressureDataType.h"
#include "UkatonSensorDataRate.h"
#include "UkatonSensorDataConfigurationsManager.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(UkatonSensorDataConfigurationsManager, Log, All);

USTRUCT(BlueprintType)
struct FUkatonSensorDataConfigurationsManager
{
    GENERATED_BODY()

    void UpdateDeviceType(EUkatonDeviceType NewDeviceType);

    const TArray<uint8> &SerializeConfigurations();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ukaton Sensor Data Configuration")
    TMap<EUkatonMotionDataType, EUkatonSensorDataRate> MotionDataRates;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ukaton Sensor Data Configuration")
    TMap<EUkatonPressureDataType, EUkatonSensorDataRate> PressureDataRates;

    FUkatonSensorDataConfigurationsManager()
    {
        SerializedConfigurations.Empty(MaxSerializedConfigurationsLength);
        TempSerializedConfiguration.Empty(MaxSerializedConfigurationLength);
    };

    void ParseConfigurations(const TArray<uint8> &Data, uint8 &Offset);

private:
    static const uint8 MaxSerializedConfigurationsLength = (2 * 2) + (3 * ((uint8)EUkatonMotionDataType::COUNT + (uint8)EUkatonPressureDataType::COUNT));
    static const uint8 MaxSerializedConfigurationLength = 2 + (3 * FMath::Max((uint8)EUkatonMotionDataType::COUNT, (uint8)EUkatonPressureDataType::COUNT));
    EUkatonDeviceType DeviceType;
    TArray<uint8> TempSerializedConfiguration;
    TArray<uint8> SerializedConfigurations;

    void SerializeConfiguration(EUkatonSensorType SensorType);
};
