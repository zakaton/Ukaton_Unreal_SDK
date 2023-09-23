// Copyright (c) 2023 Zack Qattan

#include "UkatonPressureValuesWrapper.h"
#include "UkatonPressureData.h"

DEFINE_LOG_CATEGORY(LogUkatonPressureValuesWrapper);

const FVector2D FUkatonPressureValuesWrapper::PressurePositions[NumberOfPressureSensors] = {
    FVector2D(0.6385579634772724, 0.12185506415310729),
    FVector2D(0.3549331417480725, 0.15901519981589698),
    FVector2D(0.7452523671145329, 0.20937944459744443),
    FVector2D(0.4729939843657848, 0.24446464882728644),
    FVector2D(0.21767802953129523, 0.27125012732533793),
    FVector2D(0.6841309499554993, 0.305958071294644),
    FVector2D(0.4443634258018164, 0.34255231656662977),
    FVector2D(0.2058826683251659, 0.3878235478309421),
    FVector2D(0.5179235875054955, 0.4515805318615153),
    FVector2D(0.19087039042645593, 0.49232463999939635),
    FVector2D(0.4643083092958169, 0.6703914829723581),
    FVector2D(0.19301500155484305, 0.6677506611486066),
    FVector2D(0.4643083092958169, 0.7567840826350875),
    FVector2D(0.19301500155484305, 0.7545205210718718),
    FVector2D(0.46645292042420405, 0.9129698304969649),
    FVector2D(0.19891268215790772, 0.9133470907575008)};

void FUkatonPressureValuesWrapper::UpdateDeviceType(EUkatonDeviceType NewDeviceType)
{
    DeviceType = NewDeviceType;
    UpdatePressureValuePositions();
}

void FUkatonPressureValuesWrapper::UpdatePressureValuePositions()
{
    for (uint8 i = 0; i < NumberOfPressureSensors; i++)
    {
        auto &PressureValue = PressureValues[i];
        PressureValue.Position.X = PressurePositions[i].X;
        if (DeviceType == EUkatonDeviceType::RIGHT_INSOLE)
        {
            PressureValue.Position.X = 1 - PressureValue.Position.X;
        }
        PressureValue.Position.Y = PressurePositions[i].Y;
    }
}

void FUkatonPressureValuesWrapper::ParseData(const TArray<uint8> &Data, uint8 &Offset, const EUkatonPressureDataType PressureDataType)
{
    RawValueSum = 0;

    auto bIsSingleByte = PressureDataType == EUkatonPressureDataType::PRESSURE_SINGLE_BYTE;
    auto Scalar = FUkatonPressureData::ScalarMap[PressureDataType];

    uint8 OffsetIncrement = bIsSingleByte ? 1 : 2;
    for (uint8 i = 0; i < NumberOfPressureSensors; i++)
    {
        auto &PressureValue = PressureValues[i];
        if (bIsSingleByte)
        {
            PressureValue.RawValue = Data[Offset];
        }
        else
        {
            PressureValue.RawValue = ByteParser::GetUint16(Data, Offset);
        }
        PressureValue.NormalizedValue = PressureValue.RawValue * Scalar;
        RawValueSum += PressureValue.RawValue;
        Offset += OffsetIncrement;
    }

    CenterOfMass.Set(0, 0);
    HeelToToe = 0;

    if (RawValueSum > 0)
    {
        for (uint8 i = 0; i < NumberOfPressureSensors; i++)
        {
            auto &PressureValue = PressureValues[i];
            PressureValue.WeightedValue = PressureValue.RawValue / RawValueSum;
            CenterOfMass += PressureValue.Position * PressureValue.WeightedValue;
        }

        CenterOfMass.Y = 1 - CenterOfMass.Y;
        HeelToToe = CenterOfMass.Y;
    }

    Mass = RawValueSum * Scalar / NumberOfPressureSensors;
}