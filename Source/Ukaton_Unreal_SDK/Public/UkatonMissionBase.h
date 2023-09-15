// Copyright (c) 2023 Zack Qattan

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UkatonDeviceType.h"
#include "UkatonSensorDataConfigurationsManager.h"
#include "UkatonSensorDataManager.h"
#include "UkatonHapticsManager.h"
#include "UkatonMissionBase.generated.h"

UCLASS(Abstract)
class UKATON_UNREAL_SDK_API AUkatonMissionBase : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AUkatonMissionBase();

	UPROPERTY(BlueprintReadOnly, Category = "Ukaton Mission")
	EUkatonDeviceType DeviceType;

	UPROPERTY(BlueprintReadOnly, Category = "Ukaton Mission")
	bool bIsConnected;

	UPROPERTY(BlueprintReadOnly, Category = "Ukaton Mission")
	FString DeviceName;

	UPROPERTY(BlueprintReadOnly, Category = "Ukaton Mission")
	uint8 BatteryLevel;

	UPROPERTY(BlueprintReadOnly, Category = "Ukaton Mission")
	FUkatonSensorDataConfigurationsManager SensorDataConfigurationsManager;

	UPROPERTY(BlueprintReadOnly, Category = "Ukaton Mission")
	FUkatonSensorDataManager SensorDataManager;

	FUkatonHapticsManager HapticsManager;

	virtual void Connect(const FString &DeviceIdentifier);
	virtual void Disconnect();

protected:
	// Called when the game starts or when spawned
	void BeginPlay() override;

private:
	void UpdateDeviceType(EUkatonDeviceType NewDeviceType);
	void UpdateDeviceName(FString NewDeviceName);

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};