// Copyright (c) 2023 Zack Qattan

#include "UkatonPressureData.h"
#include "ByteParser.h"

#define ENABLE_UKATON_PRESSURE_DATA_LOGGING true

#if ENABLE_UKATON_PRESSURE_DATA_LOGGING
#define UKATON_PRESSURE_DATA_LOG_VERBOSITY Log
#else
#define UKATON_PRESSURE_DATA_LOG_VERBOSITY None
#endif

DECLARE_LOG_CATEGORY_EXTERN(UkatonPressureData, Log, All);

const TMap<EUkatonPressureDataType, double> FUkatonPressureData::ScalarMap = {
    {EUkatonPressureDataType::PRESSURE_SINGLE_BYTE, 1 / FMath::Pow(2.0f, 8.0f)},
    {EUkatonPressureDataType::PRESSURE_DOUBLE_BYTE, 1 / FMath::Pow(2.0f, 12.0f)},
    {EUkatonPressureDataType::CENTER_OF_MASS, 1},
    {EUkatonPressureDataType::MASS, 1 / FMath::Pow(2.0f, 16.0f)},
    {EUkatonPressureDataType::HEEL_TO_TOE, 1}};

void FUkatonPressureData::SetDeviceType(EUkatonDeviceType NewDeviceType)
{
    DeviceType = NewDeviceType;
    PressureValuesWrapper.SetDeviceType(DeviceType);
}

uint8 FUkatonPressureData::ParseData(const TArray<uint8> &Data, uint8 Offset, uint8 FinalByteOffset)
{
    while (Offset < FinalByteOffset)
    {
        auto PressureDataType = (EUkatonPressureDataType)Data[Offset++];
        UE_LOG(UkatonPressureData, UKATON_PRESSURE_DATA_LOG_VERBOSITY, TEXT("PressureDataType: %d"), static_cast<uint8>(PressureDataType));

        auto Scalar = ScalarMap.Find(PressureDataType);

        switch (PressureDataType)
        {
        case EUkatonPressureDataType::PRESSURE_SINGLE_BYTE:
        case EUkatonPressureDataType::PRESSURE_DOUBLE_BYTE:
            Offset = PressureValuesWrapper.ParseData(Data, Offset, PressureDataType);

            CenterOfMass = PressureValuesWrapper.CenterOfMass;
            DataUpdateFlags.SetFlag(EUkatonPressureDataType::CENTER_OF_MASS);

            Mass = PressureValuesWrapper.Mass;
            DataUpdateFlags.SetFlag(EUkatonPressureDataType::MASS);

            HeelToToe = PressureValuesWrapper.HeelToToe;
            DataUpdateFlags.SetFlag(EUkatonPressureDataType::HEEL_TO_TOE);
            break;
        case EUkatonPressureDataType::CENTER_OF_MASS:
            CenterOfMass.Set(
                ByteParser::GetFloat(Data, Offset),
                ByteParser::GetFloat(Data, Offset + 4));
            Offset += 2 * 4; // sizeof(float)
            break;
        case EUkatonPressureDataType::MASS:
            Mass = ByteParser::GetUint32(Data, Offset);
            Offset += 4; // sizeof(uint32)
            break;
        case EUkatonPressureDataType::HEEL_TO_TOE:
            HeelToToe = ByteParser::GetDouble(Data, Offset);
            Offset += 8; // sizeof(double)
            break;
        default:
            UE_LOG(UkatonPressureData, Error, TEXT("Uncaught handler for PressureDataType: %d"), static_cast<uint8>(PressureDataType));
            break;
        }

        DataUpdateFlags.SetFlag(PressureDataType);
    }
    return Offset;
}