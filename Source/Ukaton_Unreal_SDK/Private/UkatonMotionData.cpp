// Copyright (c) 2023 Zack Qattan

#include "UkatonMotionData.h"
#include "ByteParser.h"
#include "Math/UnrealMathUtility.h"

#define ENABLE_UKATON_MOTION_DATA_LOGGING true

#if ENABLE_UKATON_MOTION_DATA_LOGGING
#define UKATON_MOTION_DATA_LOG_VERBOSITY Log
#else
#define UKATON_MOTION_DATA_LOG_VERBOSITY None
#endif

DECLARE_LOG_CATEGORY_EXTERN(UkatonMotionData, Log, All);

const TMap<EUkatonMotionDataType, double> FUkatonMotionData::ScalarMap = {
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
        UE_LOG(UkatonMotionData, UKATON_MOTION_DATA_LOG_VERBOSITY, TEXT("MotionDataType: %d"), static_cast<uint8>(MotionDataType));

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
            UE_LOG(UkatonMotionData, Error, TEXT("Uncaught handler for MotionDataType: %d"), static_cast<uint8>(MotionDataType));
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

#if ENABLE_UKATON_MOTION_DATA_LOGGING
    UE_LOG(UkatonMotionData, UKATON_MOTION_DATA_LOG_VERBOSITY, TEXT("MotionDataType %d: %s"), static_cast<uint8>(MotionDataType), *TempVector.ToString());
#endif

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
        UE_LOG(UkatonMotionData, Error, TEXT("Uncaught handler for MotionDataType: %d"), MotionDataType);
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

#if ENABLE_UKATON_MOTION_DATA_LOGGING
    UE_LOG(UkatonMotionData, UKATON_MOTION_DATA_LOG_VERBOSITY, TEXT("MotionDataType %d: %s"), static_cast<uint8>(EUkatonMotionDataType::ROTATION_RATE), *TempVector.ToString());
#endif

    RotationRate = TempVector;
}

const TMap<EUkatonDeviceType, FQuat> FUkatonMotionData::InitializeCorrectionQuaternions()
{
    // FIX
    TMap<EUkatonDeviceType, FQuat> Quaternions;

    auto Quaternion = FQuat::MakeFromEuler(FVector(0, 0, 0));
    Quaternions.Emplace(EUkatonDeviceType::MOTION_MODULE, Quaternion);

    Quaternion = FQuat::MakeFromEuler(FVector(FMath::RadiansToDegrees(0), 0, 0));
    Quaternion *= FQuat::MakeFromEuler(FVector(FMath::RadiansToDegrees(0), 0, 0));
    Quaternions.Emplace(EUkatonDeviceType::LEFT_INSOLE, Quaternion);

    Quaternion = FQuat::MakeFromEuler(FVector(FMath::RadiansToDegrees(0), 0, 0));
    Quaternion *= FQuat::MakeFromEuler(FVector(FMath::RadiansToDegrees(0), 0, 0));
    Quaternions.Emplace(EUkatonDeviceType::RIGHT_INSOLE, Quaternion);

    return Quaternions;
}
const TMap<EUkatonDeviceType, FQuat> FUkatonMotionData::InitializeInsoleCorrectionQuaternions()
{
    // FIX
    TMap<EUkatonDeviceType, FQuat> Quaternions;

    auto Quaternion = FQuat::MakeFromEuler(FVector(0, 0, 0));
    Quaternions.Emplace(EUkatonDeviceType::MOTION_MODULE, Quaternion);

    Quaternion = FQuat::MakeFromEuler(FVector(FMath::RadiansToDegrees(0), 0, 0));
    Quaternion *= FQuat::MakeFromEuler(FVector(FMath::RadiansToDegrees(0), 0, 0));
    Quaternions.Emplace(EUkatonDeviceType::LEFT_INSOLE, Quaternion);

    Quaternion = FQuat::MakeFromEuler(FVector(FMath::RadiansToDegrees(0), 0, 0));
    Quaternion *= FQuat::MakeFromEuler(FVector(FMath::RadiansToDegrees(0), 0, 0));
    Quaternions.Emplace(EUkatonDeviceType::RIGHT_INSOLE, Quaternion);

    return Quaternions;
}

const TMap<EUkatonDeviceType, FQuat> FUkatonMotionData::CorrectionQuaternions = FUkatonMotionData::InitializeCorrectionQuaternions();
const TMap<EUkatonDeviceType, FQuat> FUkatonMotionData::InsoleCorrectionQuaternions = FUkatonMotionData::InitializeInsoleCorrectionQuaternions();

void FUkatonMotionData::ParseQuaternion(const TArray<uint8> &Data, uint8 Offset)
{
    auto Scalar = ScalarMap[EUkatonMotionDataType::QUATERNION];

    auto W = ByteParser::GetInt16(Data, Offset);
    auto X = ByteParser::GetInt16(Data, Offset + 2);
    auto Y = ByteParser::GetInt16(Data, Offset + 4);
    auto Z = ByteParser::GetInt16(Data, Offset + 6);

    // FIX
    SetQuat(TempQuaternion, W, X, Y, Z);

    TempQuaternion *= CorrectionQuaternion;

    TempQuaternion *= Scalar;

#if ENABLE_UKATON_MOTION_DATA_LOGGING
    UE_LOG(UkatonMotionData, UKATON_MOTION_DATA_LOG_VERBOSITY, TEXT("MotionDataType %d: %s"), static_cast<uint8>(EUkatonMotionDataType::QUATERNION), *TempQuaternion.ToString());
#endif

    Quaternion = TempQuaternion;
}

void FUkatonMotionData::SetQuat(FQuat &Quat, float W, float X, float Y, float Z)
{
    Quat.W = W;
    Quat.X = X;
    Quat.Y = Y;
    Quat.Z = Z;
}