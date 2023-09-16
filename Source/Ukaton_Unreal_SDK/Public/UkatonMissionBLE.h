// Copyright (c) 2023 Zack Qattan

#pragma once

#include "CoreMinimal.h"
#include "UkatonMissionBase.h"
#include "UkatonMissionBLE.generated.h"

UCLASS()
class UKATON_UNREAL_SDK_API AUkatonMissionBLE : public AUkatonMissionBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Ukaton Mission BLE")
	void Connect(const FString &BLEName) override;

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Ukaton Mission BLE")
	void Disconnect() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ukaton Mission BLE", DisplayName = "Device Name")
	FString AutoConnectBLEDeviceName = "";

	virtual FString GetAutoConnectDeviceIdentifier() const override
	{
		return AutoConnectBLEDeviceName;
	};
};
