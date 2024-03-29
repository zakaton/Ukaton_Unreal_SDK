// Copyright (c) 2023 Zack Qattan

#pragma once

#include "CoreMinimal.h"
#include "UkatonDeviceType.h"
#include "UkatonSensorType.h"
#include "UkatonMotionDataType.h"
#include "UkatonPressureDataType.h"
#include "UkatonSensorDataRate.h"
#include "UkatonSensorDataConfigurationsManager.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogUkatonSensorDataConfigurationsManager, Log, Warning);

USTRUCT(BlueprintType)
struct FUkatonSensorDataConfigurationsManager
{
    GENERATED_BODY()

    void UpdateDeviceType(EUkatonDeviceType NewDeviceType);

    TArray<uint8> SerializedConfigurations;
    void SerializeConfigurations();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ukaton Sensor Data Configuration")
    TMap<EUkatonMotionDataType, EUkatonSensorDataRate> MotionDataRates;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ukaton Sensor Data Configuration")
    TMap<EUkatonPressureDataType, EUkatonSensorDataRate> PressureDataRates;

    FUkatonSensorDataConfigurationsManager()
    {
        SerializedConfigurations.Empty(MaxSerializedConfigurationsLength + 2);
        TempSerializedConfiguration.Empty(MaxSerializedConfigurationLength + 2);
    };

    void ParseConfigurations(const TArray<uint8> &Data, uint8 &Offset);
    void ParseConfigurations(const TArray<uint8> &Data) {
        uint8 Offset = 0;
        ParseConfigurations(Data, Offset);
    }

    bool IsConfigurationNonZero = false;

private:
    static const uint8 MaxSerializedConfigurationsLength = (2 * 2) + (3 * ((uint8)EUkatonMotionDataType::COUNT + (uint8)EUkatonPressureDataType::COUNT));
    static const uint8 MaxSerializedConfigurationLength = 2 + (3 * FMath::Max((uint8)EUkatonMotionDataType::COUNT, (uint8)EUkatonPressureDataType::COUNT));
    EUkatonDeviceType DeviceType;
    TArray<uint8> TempSerializedConfiguration;

    void SerializeConfiguration(EUkatonSensorType SensorType);
};
