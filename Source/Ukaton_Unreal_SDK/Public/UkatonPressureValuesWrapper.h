// Copyright (c) 2023 Zack Qattan

#pragma once

#include "CoreMinimal.h"
#include "Math/Vector2D.h"
#include "UkatonPressureValue.h"
#include "UkatonDeviceType.h"
#include "UkatonPressureDataType.h"
#include "ByteParser.h"
#include "UkatonPressureValuesWrapper.generated.h"

USTRUCT(BlueprintType)
struct FUkatonPressureValuesWrapper
{
	GENERATED_BODY()

	static const uint8 NumberOfPressureSensors = 16;
	static const FVector2D PressurePositions[NumberOfPressureSensors];

	UPROPERTY(BlueprintReadOnly, Category = "Ukaton Pressure Values")
	TArray<FUkatonPressureValue> PressureValues;

	FUkatonPressureValuesWrapper()
	{
		PressureValues.SetNum(NumberOfPressureSensors);
	};

	EUkatonDeviceType DeviceType;
	void UpdateDeviceType(EUkatonDeviceType NewDeviceType);

	uint32 RawValueSum;
	FVector2D CenterOfMass;
	float Mass;
	double HeelToToe;

	void ParseData(const TArray<uint8> &Data, uint8 &Offset, const EUkatonPressureDataType PressureDataType);

private:
	void UpdatePressureValuePositions();
};