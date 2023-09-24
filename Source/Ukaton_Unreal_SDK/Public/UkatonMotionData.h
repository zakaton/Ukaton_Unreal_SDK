// Copyright (c) 2023 Zack Qattan

#pragma once

#include "CoreMinimal.h"
#include "Math/Quat.h"
#include "Math/Vector.h"
#include "Math/Rotator.h"
#include "UkatonDeviceType.h"
#include "UkatonMotionDataType.h"
#include "UkatonMotionCalibrationType.h"
#include "UkatonMotionCalibrationStatusType.h"
#include "EnumFlagManager.h"
#include "UkatonMotionData.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogUkatonMotionData, Log, All);

USTRUCT(BlueprintType)
struct FUkatonMotionData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "Ukaton Motion Data")
	FVector Acceleration;

	UPROPERTY(BlueprintReadOnly, Category = "Ukaton Motion Data")
	FVector Gravity;

	UPROPERTY(BlueprintReadOnly, Category = "Ukaton Motion Data")
	FVector LinearAcceleration;

	UPROPERTY(BlueprintReadOnly, Category = "Ukaton Motion Data")
	FVector RotationRate;

	UPROPERTY(BlueprintReadOnly, Category = "Ukaton Motion Data")
	FVector Magnetometer;

	UPROPERTY(BlueprintReadOnly, Category = "Ukaton Motion Data")
	FQuat Quaternion;

	UPROPERTY(BlueprintReadOnly, Category = "Ukaton Motion Data")
	FVector Euler;

	void UpdateDeviceType(EUkatonDeviceType NewDeviceType);
	void ParseData(const TArray<uint8> &Data, uint8 &Offset, const uint8 FinalOffset);
	UEnumFlagManager<EUkatonMotionDataType> DataUpdateFlags;

	UPROPERTY(BlueprintReadOnly, Category = "Ukaton Motion Data")
	TMap<EUkatonMotionCalibrationType, EUkatonMotionCalibrationStatusType> Calibration;
	void ParseCalibration(const TArray<uint8> &Data, uint8 &Offset);

	UPROPERTY(BlueprintReadOnly, Category = "Ukaton Motion Data")
	bool bIsFullyCalibrated;

	static const TMap<EUkatonMotionDataType, double> ScalarMap;

private:
	EUkatonDeviceType DeviceType;

	FVector TempVector;
	void ParseVector(const TArray<uint8> &Data, uint8 &Offset, EUkatonMotionDataType MotionDataType);
	void ParseEuler(const TArray<uint8> &Data, uint8 &Offset);

	FQuat TempQuaternion;
	FQuat CorrectionQuaternion;
	void ParseQuaternion(const TArray<uint8> &Data, uint8 &Offset);

	void SetQuat(FQuat &Quat, float W, float X, float Y, float Z);

	static const TMap<EUkatonDeviceType, FQuat> CorrectionQuaternions;
	static const TMap<EUkatonDeviceType, FQuat> InsoleCorrectionQuaternions;

	static const TMap<EUkatonDeviceType, FQuat> InitializeCorrectionQuaternions();
	static const TMap<EUkatonDeviceType, FQuat> InitializeInsoleCorrectionQuaternions();
};