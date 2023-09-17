// Copyright (c) 2023 Zack Qattan

#pragma once

#include "CoreMinimal.h"
#include "Math/Vector2D.h"
#include "UkatonDeviceType.h"
#include "UkatonPressureDataType.h"
#include "UkatonPressureValuesWrapper.h"
#include "EnumFlagManager.h"
#include "UkatonPressureData.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(UkatonPressureData, Log, All);

USTRUCT(BlueprintType)
struct FUkatonPressureData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "Ukaton Pressure Data")
	FUkatonPressureValuesWrapper PressureValuesWrapper;

	UPROPERTY(BlueprintReadOnly, Category = "Ukaton Pressure Data")
	FVector2D CenterOfMass;

	UPROPERTY(BlueprintReadOnly, Category = "Ukaton Pressure Data")
	float Mass;

	UPROPERTY(BlueprintReadOnly, Category = "Ukaton Pressure Data")
	double HeelToToe;

	void UpdateDeviceType(EUkatonDeviceType NewDeviceType);
	void ParseData(const TArray<uint8> &Data, uint8 &Offset, const uint8 FinalOffset);
	UEnumFlagManager<EUkatonPressureDataType> DataUpdateFlags;

	static const TMap<EUkatonPressureDataType, double> ScalarMap;

private:
	EUkatonDeviceType DeviceType;
};
