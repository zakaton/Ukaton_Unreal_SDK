// Copyright (c) 2023 Zack Qattan

#pragma once

#include "CoreMinimal.h"
#include "Math/Quat.h"
#include "Math/Vector.h"
#include "Math/Rotator.h"
#include "UkatonMotionData.generated.h"

USTRUCT(BlueprintType)
struct FUkatonMotionData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "Ukaton")
	FVector Acceleration;

	UPROPERTY(BlueprintReadOnly, Category = "Ukaton")
	FVector Gravity;

	UPROPERTY(BlueprintReadOnly, Category = "Ukaton")
	FVector LinearAcceleration;

	UPROPERTY(BlueprintReadOnly, Category = "Ukaton")
	FVector RotationRate;

	UPROPERTY(BlueprintReadOnly, Category = "Ukaton")
	FVector Magnetometer;

	UPROPERTY(BlueprintReadOnly, Category = "Ukaton")
	FQuat Quaternion;

	UPROPERTY(BlueprintReadOnly, Category = "Ukaton")
	FVector Euler;
};
