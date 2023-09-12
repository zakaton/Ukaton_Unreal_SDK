// Copyright (c) 2023 Zack Qattan

#include "UkatonPressureData.h"

const TMap<EUkatonPressureDataType, float> FUkatonPressureData::ScalarMap = {
    {EUkatonPressureDataType::PRESSURE_SINGLE_BYTE, 1 / FMath::Pow(2.0f, 8.0f)},
    {EUkatonPressureDataType::PRESSURE_DOUBLE_BYTE, 1 / FMath::Pow(2.0f, 12.0f)},
    {EUkatonPressureDataType::MASS, 1 / FMath::Pow(2.0f, 16.0f)},
};

uint8 FUkatonPressureData::ParseData(const TArray<uint8> &Data, uint8 Offset) {
    // FILL
    return Offset;
}