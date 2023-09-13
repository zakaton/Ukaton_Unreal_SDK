// Copyright (c) 2023 Zack Qattan

#include "UkatonMotionData.h"
#include "ByteParser.h"

#define ENABLE_UKATON_MOTION_DATA_LOGGING true

#if ENABLE_UKATON_MOTION_DATA_LOGGING
#define UKATON_MOTION_DATA_LOG_VERBOSITY Log
#else
#define UKATON_MOTION_DATA_LOG_VERBOSITY None
#endif

DECLARE_LOG_CATEGORY_EXTERN(UkatonMotionData, Log, All);

const TMap<EUkatonMotionDataType, float> FUkatonMotionData::ScalarMap = {
    {EUkatonMotionDataType::ACCELERATION, FMath::Pow(2.0f, -8.0f)},
    {EUkatonMotionDataType::GRAVITY, FMath::Pow(2.0f, -8.0f)},
    {EUkatonMotionDataType::LINEAR_ACCELERATION, FMath::Pow(2.0f, -8.0f)},
    {EUkatonMotionDataType::ROTATION_RATE, FMath::Pow(2.0f, -9.0f)},
    {EUkatonMotionDataType::MAGNETOMETER, FMath::Pow(2.0f, -4.0f)},
    {EUkatonMotionDataType::QUATERNION, FMath::Pow(2.0f, -14.0f)},
};

void FUkatonMotionData::SetDeviceType(EUkatonDeviceType NewDeviceType)
{
    DeviceType = NewDeviceType;

    CorrectionQuaternion = CorrectionQuaternions[DeviceType];
    if (InsoleCorrectionQuaternions.Contains(DeviceType))
    {
        CorrectionQuaternion *= InsoleCorrectionQuaternions[DeviceType];
    }
}

uint8 FUkatonMotionData::ParseData(const TArray<uint8> &Data, uint8 Offset, uint8 FinalByteOffset)
{
    while (Offset < FinalByteOffset)
    {
        auto MotionDataType = (EUkatonMotionDataType)Data[Offset++];
        UE_LOG(UkatonMotionData, UKATON_MOTION_DATA_LOG_VERBOSITY, TEXT("MotionDataType: %u"), MotionDataType);

        switch (MotionDataType)
        {
        case EUkatonMotionDataType::ACCELERATION:
        case EUkatonMotionDataType::GRAVITY:
        case EUkatonMotionDataType::LINEAR_ACCELERATION:
        case EUkatonMotionDataType::MAGNETOMETER:
            ParseVector(Data, Offset, MotionDataType);
            Offset += 6;
            break;
        case EUkatonMotionDataType::ROTATION_RATE:
            ParseEuler(Data, Offset);
            Offset += 6;
            break;
        case EUkatonMotionDataType::QUATERNION:
            ParseQuaternion(Data, Offset);
            Offset += 8;
            break;
        default:
            UE_LOG(UkatonMotionData, Error, TEXT("Uncaught handler for MotionDataType: %u"), MotionDataType);
            break;
        }

        DataUpdateFlags.SetFlag(MotionDataType);
    }
    return Offset;
}

void FUkatonMotionData::ParseVector(const TArray<uint8> &Data, uint8 Offset, EUkatonMotionDataType MotionDataType)
{
    auto Scalar = ScalarMap[MotionDataType];
    auto X = ByteParser::GetInt16(Data, Offset);
    auto Y = ByteParser::GetInt16(Data, Offset + 2);
    auto Z = ByteParser::GetInt16(Data, Offset + 4);

    // FIX
    if (DeviceType == EUkatonDeviceType::MOTION_MODULE)
    {
        TempVector.Set(X, Y, Z);
    }
    else
    {
        if (DeviceType == EUkatonDeviceType::LEFT_INSOLE)
        {
            TempVector.Set(X, Y, Z);
        }
        else
        {
            TempVector.Set(X, Y, Z);
        }
    }

    TempVector *= Scalar;

    switch (MotionDataType)
    {
    case EUkatonMotionDataType::ACCELERATION:
        Acceleration = TempVector;
        break;
    case EUkatonMotionDataType::GRAVITY:
        Gravity = TempVector;
        break;
    case EUkatonMotionDataType::LINEAR_ACCELERATION:
        LinearAcceleration = TempVector;
        break;
    case EUkatonMotionDataType::MAGNETOMETER:
        Magnetometer = TempVector;
        break;
    default:
        UE_LOG(UkatonMotionData, Error, TEXT("Uncaught handler for MotionDataType: %u"), MotionDataType);
        break;
    }
}
void FUkatonMotionData::ParseEuler(const TArray<uint8> &Data, uint8 Offset)
{
    auto Scalar = ScalarMap[EUkatonMotionDataType::ROTATION_RATE];
    auto X = ByteParser::GetInt16(Data, Offset);
    auto Y = ByteParser::GetInt16(Data, Offset + 2);
    auto Z = ByteParser::GetInt16(Data, Offset + 4);

    // FIX
    if (DeviceType == EUkatonDeviceType::MOTION_MODULE)
    {
        TempVector.Set(X, Y, Z);
    }
    else
    {
        if (DeviceType == EUkatonDeviceType::LEFT_INSOLE)
        {
            TempVector.Set(X, Y, Z);
        }
        else
        {
            TempVector.Set(X, Y, Z);
        }
    }

    TempVector *= Scalar;

    RotationRate = TempVector;
}

const TMap<EUkatonDeviceType, FQuat> FUkatonMotionData::CorrectionQuaternions = []
{
    TMap<EUkatonDeviceType, FQuat> Quaternions;
    return Quaternions;
}();
const TMap<EUkatonDeviceType, FQuat> FUkatonMotionData::InsoleCorrectionQuaternions = []
{
    TMap<EUkatonDeviceType, FQuat> Quaternions;
    return Quaternions;
}();

void FUkatonMotionData::ParseQuaternion(const TArray<uint8> &Data, uint8 Offset)
{
    auto scalar = ScalarMap[EUkatonMotionDataType::QUATERNION];

    auto W = ByteParser::GetInt16(Data, Offset);
    auto X = ByteParser::GetInt16(Data, Offset + 2);
    auto Y = ByteParser::GetInt16(Data, Offset + 4);
    auto Z = ByteParser::GetInt16(Data, Offset + 6);

    // FIX
    SetQuat(TempQuaternion, W, X, Y, Z);

    TempQuaternion *= CorrectionQuaternion;

    TempQuaternion *= scalar;

    Quaternion = TempQuaternion;
}

void FUkatonMotionData::SetQuat(FQuat &Quat, float W, float X, float Y, float Z)
{
    Quat.W = W;
    Quat.X = X;
    Quat.Y = Y;
    Quat.Z = Z;
}