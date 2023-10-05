// Copyright (c) 2023 Zack Qattan + ChatGPT

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "UDPPortUtils.generated.h"

UCLASS()
class UKATONMISSION_API UUDPPortUtils : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "UDP Port Utils")
	static bool FindAvailableUdpPort(int32 &OutPort);
};
