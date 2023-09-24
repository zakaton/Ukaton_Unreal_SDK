// Copyright (c) 2023 Zack Qattan

#include "UkatonMotionData.h"
#include "ByteParser.h"
#include "Math/UnrealMathUtility.h"
#include "Logging/StructuredLog.h"

DEFINE_LOG_CATEGORY(LogUkatonMotionData);

const TMap<EUkatonMotionDataType, double> FUkatonMotionData::ScalarMap = {
    {EUkatonMotionDataType::ACCELERATION, FMath::Pow(2.0f, -8.0f)},
    {EUkatonMotionDataType::GRAVITY, FMath::Pow(2.0f, -8.0f)},
    {EUkatonMotionDataType::LINEAR_ACCELERATION, FMath::Pow(2.0f, -8.0f)},
    {EUkatonMotionDataType::ROTATION_RATE, FMath::Pow(2.0f, -9.0f)},
    {EUkatonMotionDataType::MAGNETOMETER, FMath::Pow(2.0f, -4.0f)},
    {EUkatonMotionDataType::QUATERNION, FMath::Pow(2.0f, -14.0f)},
};

void FUkatonMotionData::UpdateDeviceType(EUkatonDeviceType NewDeviceType)
{
    DeviceType = NewDeviceType;

    CorrectionQuaternion = CorrectionQuaternions[DeviceType];
    if (InsoleCorrectionQuaternions.Contains(DeviceType))
    {
        CorrectionQuaternion *= InsoleCorrectionQuaternions[DeviceType];
    }
}

void FUkatonMotionData::ParseData(const TArray<uint8> &Data, uint8 &Offset, const uint8 FinalOffset)
{
    while (Offset < FinalOffset)
    {
        auto MotionDataType = (EUkatonMotionDataType)Data[Offset++];
        UE_LOGFMT(LogUkatonMotionData, Log, "MotionDataType {0}", static_cast<uint8>(MotionDataType));

        switch (MotionDataType)
        {
        case EUkatonMotionDataType::ACCELERATION:
        case EUkatonMotionDataType::GRAVITY:
        case EUkatonMotionDataType::LINEAR_ACCELERATION:
        case EUkatonMotionDataType::MAGNETOMETER:
            ParseVector(Data, Offset, MotionDataType);
            break;
        case EUkatonMotionDataType::ROTATION_RATE:
            ParseEuler(Data, Offset);
            break;
        case EUkatonMotionDataType::QUATERNION:
            ParseQuaternion(Data, Offset);
            break;
        default:
            UE_LOGFMT(LogUkatonMotionData, Error, "Uncaught handler for MotionDataType: {0}", static_cast<uint8>(MotionDataType));
            break;
        }

        DataUpdateFlags.SetFlag(MotionDataType);
    }
}

void FUkatonMotionData::ParseCalibration(const TArray<uint8> &Data, uint8 &Offset)
{
    bool bNewIsFullyCalibrated = true;
    for (uint8 CalibrationTypeIndex = 0; CalibrationTypeIndex < static_cast<uint8>(EUkatonMotionCalibrationType::COUNT); CalibrationTypeIndex++)
    {
        auto CalibrationType = static_cast<EUkatonMotionCalibrationType>(CalibrationTypeIndex);
        auto CalibrationStatusValue = Data[Offset++];
        auto CalibrationStatus = static_cast<EUkatonMotionCalibrationStatusType>(CalibrationStatusValue);
        Calibration.Emplace(CalibrationType, CalibrationStatus);
        bNewIsFullyCalibrated = bNewIsFullyCalibrated && CalibrationStatus == EUkatonMotionCalibrationStatusType::ACCURACY_HIGH;
    }
    bIsFullyCalibrated = bNewIsFullyCalibrated;
}

void FUkatonMotionData::ParseVector(const TArray<uint8> &Data, uint8 &Offset, EUkatonMotionDataType MotionDataType)
{
    auto Scalar = ScalarMap[MotionDataType];
    auto X = ByteParser::GetInt16(Data, Offset);
    auto Y = ByteParser::GetInt16(Data, Offset + 2);
    auto Z = ByteParser::GetInt16(Data, Offset + 4);
    Offset += 6;

    if (DeviceType == EUkatonDeviceType::MOTION_MODULE)
    {
        TempVector.Set(Y, X, -Z);
    }
    else
    {
        if (DeviceType == EUkatonDeviceType::LEFT_INSOLE)
        {
            TempVector.Set(-X, Z, Y);
        }
        else
        {
            TempVector.Set(X, -Z, Y);
        }
    }

    TempVector *= Scalar;

    UE_LOGFMT(LogUkatonMotionData, Log, "MotionDataType {0} Vector: {1}", static_cast<uint8>(MotionDataType), *TempVector.ToString());

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
        UE_LOGFMT(LogUkatonMotionData, Error, "Uncaught handler for MotionDataType {0}", static_cast<uint8>(MotionDataType));
        break;
    }
}
void FUkatonMotionData::ParseEuler(const TArray<uint8> &Data, uint8 &Offset)
{
    auto Scalar = ScalarMap[EUkatonMotionDataType::ROTATION_RATE];
    auto X = ByteParser::GetInt16(Data, Offset);
    auto Y = ByteParser::GetInt16(Data, Offset + 2);
    auto Z = ByteParser::GetInt16(Data, Offset + 4);
    Offset += 6;

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
    TempRotator.MakeFromEuler(TempVector);

    UE_LOGFMT(LogUkatonMotionData, Log, "MotionDataType {0} Vector: {1}", static_cast<uint8>(EUkatonMotionDataType::ROTATION_RATE), *TempVector.ToString());

    RotationRate = TempRotator;
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

void FUkatonMotionData::ParseQuaternion(const TArray<uint8> &Data, uint8 &Offset)
{
    auto Scalar = ScalarMap[EUkatonMotionDataType::QUATERNION];

    auto W = ByteParser::GetInt16(Data, Offset);
    auto X = ByteParser::GetInt16(Data, Offset + 2);
    auto Y = ByteParser::GetInt16(Data, Offset + 4);
    auto Z = ByteParser::GetInt16(Data, Offset + 6);
    Offset += 8;

    // FIX
    // SetQuat(TempQuaternion, W, X, Y, Z);
    SetQuat(TempQuaternion, -Y, W, X, Z);

    TempQuaternion *= CorrectionQuaternion;

    TempQuaternion *= Scalar;

    UE_LOGFMT(LogUkatonMotionData, Log, "MotionDataType {0} Quaternion: {1}", static_cast<uint8>(EUkatonMotionDataType::QUATERNION), *TempQuaternion.ToString());

    Quaternion = TempQuaternion;
}

void FUkatonMotionData::SetQuat(FQuat &Quat, float W, float X, float Y, float Z)
{
    Quat.W = W;
    Quat.X = X;
    Quat.Y = Y;
    Quat.Z = Z;
}