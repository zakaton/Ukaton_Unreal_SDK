// Copyright (c) 2023 Zack Qattan

#include "UkatonPressureData.h"
#include "ByteParser.h"
#include "Logging/StructuredLog.h"

DEFINE_LOG_CATEGORY(LogUkatonPressureData);

const TMap<EUkatonPressureDataType, double> FUkatonPressureData::ScalarMap = {
    {EUkatonPressureDataType::PRESSURE_SINGLE_BYTE, 1 / FMath::Pow(2.0f, 8.0f)},
    {EUkatonPressureDataType::PRESSURE_DOUBLE_BYTE, 1 / FMath::Pow(2.0f, 12.0f)},
    {EUkatonPressureDataType::CENTER_OF_MASS, 1},
    {EUkatonPressureDataType::MASS, 1 / FMath::Pow(2.0f, 16.0f)},
    {EUkatonPressureDataType::HEEL_TO_TOE, 1}};

void FUkatonPressureData::UpdateDeviceType(EUkatonDeviceType NewDeviceType)
{
    DeviceType = NewDeviceType;
    PressureValuesWrapper.UpdateDeviceType(DeviceType);
}

void FUkatonPressureData::ParseData(const TArray<uint8> &Data, uint8 &Offset, const uint8 FinalOffset)
{
    while (Offset < FinalOffset)
    {
        auto PressureDataType = (EUkatonPressureDataType)Data[Offset++];
        UE_LOGFMT(LogUkatonPressureData, Log, "PressureDataType {0}", static_cast<uint8>(PressureDataType));

        switch (PressureDataType)
        {
        case EUkatonPressureDataType::PRESSURE_SINGLE_BYTE:
        case EUkatonPressureDataType::PRESSURE_DOUBLE_BYTE:
            PressureValuesWrapper.ParseData(Data, Offset, PressureDataType);

            CenterOfMass = PressureValuesWrapper.CenterOfMass;
            DataUpdateFlags.SetFlag(EUkatonPressureDataType::CENTER_OF_MASS);
            UE_LOGFMT(LogUkatonPressureData, Log, "CenterOfMass(pressure): {0}", *CenterOfMass.ToString());

            Mass = PressureValuesWrapper.Mass;
            DataUpdateFlags.SetFlag(EUkatonPressureDataType::MASS);
            UE_LOGFMT(LogUkatonPressureData, Log, "Mass(pressure): {0}", Mass);

            HeelToToe = PressureValuesWrapper.HeelToToe;
            DataUpdateFlags.SetFlag(EUkatonPressureDataType::HEEL_TO_TOE);
            UE_LOGFMT(LogUkatonPressureData, Log, "HeelToToe(pressure): {0}", HeelToToe);
            break;
        case EUkatonPressureDataType::CENTER_OF_MASS:
            CenterOfMass.Set(
                ByteParser::GetFloat(Data, Offset),
                ByteParser::GetFloat(Data, Offset + 4));
            UE_LOGFMT(LogUkatonPressureData, Log, "CenterOfMass: {0}", *CenterOfMass.ToString());
            Offset += 2 * 4; // sizeof(float)
            break;
        case EUkatonPressureDataType::MASS:
            Mass = ByteParser::GetUint32(Data, Offset) * ScalarMap[PressureDataType];
            UE_LOGFMT(LogUkatonPressureData, Log, "Mass: {0}", Mass);
            Offset += 4; // sizeof(uint32)
            break;
        case EUkatonPressureDataType::HEEL_TO_TOE:
            HeelToToe = ByteParser::GetDouble(Data, Offset);
            UE_LOGFMT(LogUkatonPressureData, Log, "HeelToToe: {0}", HeelToToe);
            Offset += 8; // sizeof(double)
            break;
        default:
            UE_LOGFMT(LogUkatonPressureData, Error, "Uncaught handler for PressureDataType {0}", static_cast<uint8>(PressureDataType));
            break;
        }

        DataUpdateFlags.SetFlag(PressureDataType);
    }
}