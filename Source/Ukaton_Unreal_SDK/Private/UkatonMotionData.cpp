// Copyright (c) 2023 Zack Qattan + ChatGPT

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

FUkatonMotionData::FUkatonMotionData()
{
    TempArray1.SetNum(4);
    TempArray2.SetNum(4);
}

void FUkatonMotionData::UpdateDeviceType(EUkatonDeviceType NewDeviceType)
{
    DeviceType = NewDeviceType;

    CorrectionQuaternion = CorrectionQuaternions[DeviceType];
    UE_LOGFMT(LogUkatonMotionData, Log, "CorrectionQuaternion: {0}", *CorrectionQuaternion.ToString());
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

    if (DeviceType == EUkatonDeviceType::MOTION_MODULE)
    {
        TempVector.Set(-X, -Y, -Z);
    }
    else
    {
        if (DeviceType == EUkatonDeviceType::LEFT_INSOLE)
        {
            TempVector.Set(-Z, X, Y);
        }
        else
        {
            TempVector.Set(Z, -X, Y);
        }
    }

    TempVector *= Scalar;

    SetRotator(TempRotator, TempVector);

    UE_LOGFMT(LogUkatonMotionData, Log, "MotionDataType {0} Euler: {1}", static_cast<uint8>(EUkatonMotionDataType::ROTATION_RATE), *TempRotator.ToString());

    RotationRate = TempRotator;
}

const TMap<EUkatonDeviceType, FQuat> FUkatonMotionData::InitializeCorrectionQuaternions()
{
    TMap<EUkatonDeviceType, FQuat> Quaternions;

    auto Quaternion = FQuat::MakeFromEuler(FVector(0, 0, -90));
    Quaternions.Emplace(EUkatonDeviceType::MOTION_MODULE, Quaternion);

    Quaternion = FQuat::MakeFromEuler(FVector(-90, 0, 0));
    Quaternion *= FQuat::MakeFromEuler(FVector(0, 0, 180));
    Quaternions.Emplace(EUkatonDeviceType::LEFT_INSOLE, Quaternion);

    Quaternion = FQuat::MakeFromEuler(FVector(-90, 0, 0));
    Quaternion *= FQuat::MakeFromEuler(FVector(0, 0, 0));
    Quaternions.Emplace(EUkatonDeviceType::RIGHT_INSOLE, Quaternion);

    return Quaternions;
}

// begin ChatGPT
const TArray<TArray<int8>> FUkatonMotionData::InitializeQuaternionPermutations()
{
    TArray<TArray<int8>> _QuaternionPermutations;
    TArray<int8> CurrentQuaternionPermutation;

    TArray<int8> InputArray = {1, 2, 3, 4};

    // Start the recursive permutation generation
    GeneratePermutationsAndSignVariationsRec(InputArray, _QuaternionPermutations, CurrentQuaternionPermutation, 0);

    return _QuaternionPermutations;
}
void FUkatonMotionData::GeneratePermutationsAndSignVariationsRec(
    const TArray<int8> &inputArray,
    TArray<TArray<int8>> &permutations,
    TArray<int8> &currentPermutation,
    int32 currentIndex)
{
    if (currentIndex == inputArray.Num())
    {
        // All elements have been processed, add the current permutation to the result
        permutations.Add(currentPermutation);
        return;
    }

    // Generate permutations with the current element
    for (int32 i = 0; i < 2; i++)
    {
        int8 sign = (i == 0) ? 1 : -1;
        currentPermutation.Add(inputArray[currentIndex] * sign);
        GeneratePermutationsAndSignVariationsRec(inputArray, permutations, currentPermutation, currentIndex + 1);
        currentPermutation.RemoveAt(currentPermutation.Num() - 1);
    }
}
// end ChatGPT

void FUkatonMotionData::PermuteQuaternion(FQuat &Quat)
{
    if (QuaternionPermutationIndex < 0)
    {
        QuaternionPermutationIndex = 0;
    }
    else if (QuaternionPermutationIndex >= QuaternionPermutations.Num())
    {
        QuaternionPermutationIndex %= QuaternionPermutations.Num();
    }

    auto QuaternionPermutation = QuaternionPermutations[QuaternionPermutationIndex];

    TempArray1[0] = Quat.W;
    TempArray1[1] = Quat.X;
    TempArray1[2] = Quat.Y;
    TempArray1[3] = Quat.Z;

    for (uint8 i = 0; i < QuaternionPermutation.Num(); i++)
    {
        auto j = QuaternionPermutation[i];
        auto Value = TempArray1[i];
        TempArray2[i] = TempArray1[FMath::Abs(j) - 1];
        if (j < 0)
        {
            TempArray2[i] *= -1;
        }
    }

    SetQuat(Quat, TempArray2[0], TempArray2[1], TempArray2[2], TempArray2[3]);
    // UE_LOGFMT(LogUkatonMotionData, Log, "QuaternionPermutation #{0}: {1},{2},{3},{4}", QuaternionPermutationIndex, QuaternionPermutation[0], QuaternionPermutation[1], QuaternionPermutation[2], QuaternionPermutation[3]);
}

const TMap<EUkatonDeviceType, FQuat> FUkatonMotionData::CorrectionQuaternions = FUkatonMotionData::InitializeCorrectionQuaternions();
const TArray<TArray<int8>> FUkatonMotionData::QuaternionPermutations = FUkatonMotionData::InitializeQuaternionPermutations();

void FUkatonMotionData::ParseQuaternion(const TArray<uint8> &Data, uint8 &Offset)
{
    auto Scalar = ScalarMap[EUkatonMotionDataType::QUATERNION];

    double W = ByteParser::GetInt16(Data, Offset);
    double X = ByteParser::GetInt16(Data, Offset + 2);
    double Y = ByteParser::GetInt16(Data, Offset + 4);
    double Z = ByteParser::GetInt16(Data, Offset + 6);
    Offset += 8;

    SetQuat(TempQuaternion, W, X, -Y, -Z);
    // PermuteQuaternion(TempQuaternion);

    TempQuaternion *= CorrectionQuaternion;

    TempQuaternion *= Scalar;

    UE_LOGFMT(LogUkatonMotionData, Log, "MotionDataType {0} Quaternion: {1}", static_cast<uint8>(EUkatonMotionDataType::QUATERNION), *TempQuaternion.ToString());

    Quaternion = TempQuaternion;

    auto EulerVector = Quaternion.Euler();
    SetRotator(Euler, EulerVector);
    UE_LOGFMT(LogUkatonMotionData, Log, "Euler: {0}", *Euler.ToString());
}

void FUkatonMotionData::SetQuat(FQuat &Quat, double W, double X, double Y, double Z)
{
    Quat.W = W;
    Quat.X = X;
    Quat.Y = Y;
    Quat.Z = Z;
}
void FUkatonMotionData::SetRotator(FRotator &Rotator, const FVector &Vector)
{
    Rotator.Pitch = Vector.X;
    Rotator.Roll = Vector.Y;
    Rotator.Yaw = Vector.Z;
}
