// Copyright (c) 2023 Zack Qattan

#include "UkatonSensorDataConfigurationsManager.h"
#include "Logging/StructuredLog.h"

DECLARE_LOG_CATEGORY_EXTERN(UkatonSensorDataConfigurationsManager, Log, All);

void FUkatonSensorDataConfigurationsManager::UpdateDeviceType(EUkatonDeviceType NewDeviceType)
{
    DeviceType = NewDeviceType;
}

const TArray<uint8> &FUkatonSensorDataConfigurationsManager::SerializeConfigurations()
{
    SerializedConfigurations.Reset();

    SerializeConfiguration(EUkatonSensorType::MOTION);

    if (DeviceType != EUkatonDeviceType::MOTION_MODULE)
    {
        SerializeConfiguration(EUkatonSensorType::PRESSURE);
    }

    return SerializedConfigurations;
}

void FUkatonSensorDataConfigurationsManager::SerializeConfiguration(EUkatonSensorType SensorType)
{
    TMap<uint8, uint16> *SensorDataRates = nullptr;

    switch (SensorType)
    {
    case EUkatonSensorType::MOTION:
        SensorDataRates = reinterpret_cast<TMap<uint8, uint16> *>(&MotionDataRates);
        break;
    case EUkatonSensorType::PRESSURE:
        SensorDataRates = reinterpret_cast<TMap<uint8, uint16> *>(&PressureDataRates);
        break;
    default:
        UE_LOGFMT(UkatonSensorDataConfigurationsManager, Error, "Uncaught handler for SensorType {0}", static_cast<uint8>(SensorType));
        break;
    }

    if (SensorDataRates != nullptr)
    {
        TempSerializedConfiguration.Reset();

        for (auto &SensorDataRate : *SensorDataRates)
        {
            TempSerializedConfiguration.Emplace(SensorDataRate.Key);
            TempSerializedConfiguration.Emplace(static_cast<uint8>(SensorDataRate.Value & 0xFF));
            TempSerializedConfiguration.Emplace(static_cast<uint8>((SensorDataRate.Value >> 8) & 0xFF));
        }
        if (TempSerializedConfiguration.Num() > 0)
        {
            SerializedConfigurations.Emplace((uint8)SensorType);
            SerializedConfigurations.Emplace(TempSerializedConfiguration.Num());
            SerializedConfigurations += TempSerializedConfiguration;
        }
    }
}

void FUkatonSensorDataConfigurationsManager::ParseConfigurations(const TArray<uint8> &Data, uint8 &Offset)
{
    // FILL
}