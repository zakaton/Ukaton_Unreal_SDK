// Copyright (c) 2023 Zack Qattan

#include "UkatonSensorDataConfigurationsManager.h"
#include "Logging/StructuredLog.h"
#include "ByteParser.h"

DEFINE_LOG_CATEGORY(LogUkatonSensorDataConfigurationsManager);

void FUkatonSensorDataConfigurationsManager::UpdateDeviceType(EUkatonDeviceType NewDeviceType)
{
    DeviceType = NewDeviceType;
}

void FUkatonSensorDataConfigurationsManager::SerializeConfigurations()
{
    SerializedConfigurations.Reset();

    SerializeConfiguration(EUkatonSensorType::MOTION);

    if (DeviceType != EUkatonDeviceType::MOTION_MODULE)
    {
        SerializeConfiguration(EUkatonSensorType::PRESSURE);
    }

    UE_LOGFMT(LogUkatonSensorDataConfigurationsManager, Log, "SerializedConfigurations {0}", SerializedConfigurations.Num());
    for (uint8 Index = 0; Index < SerializedConfigurations.Num(); Index++)
    {
        UE_LOGFMT(LogUkatonSensorDataConfigurationsManager, Log, "{0}:{1}", Index, SerializedConfigurations[Index]);
    }
}

void FUkatonSensorDataConfigurationsManager::SerializeConfiguration(EUkatonSensorType SensorType)
{
    TMap<uint8, uint8> *SensorDataRates = nullptr;

    switch (SensorType)
    {
    case EUkatonSensorType::MOTION:
        SensorDataRates = reinterpret_cast<TMap<uint8, uint8> *>(&MotionDataRates);
        break;
    case EUkatonSensorType::PRESSURE:
        SensorDataRates = reinterpret_cast<TMap<uint8, uint8> *>(&PressureDataRates);
        break;
    default:
        UE_LOGFMT(LogUkatonSensorDataConfigurationsManager, Error, "Uncaught handler for SensorType {0}", static_cast<uint8>(SensorType));
        break;
    }

    if (SensorDataRates != nullptr)
    {
        TempSerializedConfiguration.Reset();

        for (auto &SensorDataRate : *SensorDataRates)
        {
            TempSerializedConfiguration.Emplace(SensorDataRate.Key);
            uint16 Value = SensorDataRate.Value * 10;
            TempSerializedConfiguration.Emplace(static_cast<uint8>(Value & 0xFF));
            TempSerializedConfiguration.Emplace(static_cast<uint8>((Value >> 8) & 0xFF));
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
    bool _IsConfigurationNonZero = false;
    for (uint8 SensorTypeIndex = 0; SensorTypeIndex < static_cast<uint8>(EUkatonSensorType::COUNT); SensorTypeIndex++)
    {
        auto SensorType = static_cast<EUkatonSensorType>(SensorTypeIndex);
        switch (SensorType)
        {
        case EUkatonSensorType::MOTION:
            for (uint8 MotionDataTypeIndex = 0; MotionDataTypeIndex < static_cast<uint8>(EUkatonMotionDataType::COUNT); MotionDataTypeIndex++)
            {
                auto MotionDataType = static_cast<EUkatonMotionDataType>(MotionDataTypeIndex);
                auto RawSensorDataRate = ByteParser::GetUint16(Data, Offset);
                auto SensorDataRate = static_cast<EUkatonSensorDataRate>(RawSensorDataRate / 10);
                Offset += 2;
                MotionDataRates.Emplace(MotionDataType, SensorDataRate);

                _IsConfigurationNonZero = _IsConfigurationNonZero || (RawSensorDataRate > 0);
            }
            break;
        case EUkatonSensorType::PRESSURE:
            for (uint8 PressureDataTypeIndex = 0; PressureDataTypeIndex < static_cast<uint8>(EUkatonPressureDataType::COUNT); PressureDataTypeIndex++)
            {
                auto PressureDataType = static_cast<EUkatonPressureDataType>(PressureDataTypeIndex);
                auto RawSensorDataRate = ByteParser::GetUint16(Data, Offset);
                auto SensorDataRate = static_cast<EUkatonSensorDataRate>(RawSensorDataRate / 10);
                Offset += 2;
                PressureDataRates.Emplace(PressureDataType, SensorDataRate);

                _IsConfigurationNonZero = _IsConfigurationNonZero || (RawSensorDataRate > 0);
            }
            break;
        default:
            UE_LOGFMT(LogUkatonSensorDataConfigurationsManager, Error, "Uncaught handler for SensorType {0}", static_cast<uint8>(SensorType));
            break;
        }
    }
    IsConfigurationNonZero = _IsConfigurationNonZero;
}