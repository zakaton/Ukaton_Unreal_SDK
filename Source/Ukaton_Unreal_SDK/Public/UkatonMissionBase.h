// Copyright (c) 2023 Zack Qattan

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UkatonDeviceType.h"
#include "UkatonSensorDataConfigurationsManager.h"
#include "UkatonSensorDataManager.h"
#include "UkatonHapticsManager.h"
#include "UkatonMissionBase.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(UkatonMissionBase, Log, All);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDelegateWithNoParams);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FBatteryLevelUpdatedDelegate, uint8, BatteryLevel);

UCLASS(Abstract)
class UKATON_UNREAL_SDK_API AUkatonMissionBase : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AUkatonMissionBase();

	UFUNCTION(BlueprintPure, Category = "Ukaton Mission")
	EUkatonDeviceType GetDeviceType() const { return DeviceType; };

	UFUNCTION(BlueprintPure, Category = "Ukaton Mission")
	FString GetDeviceName() const { return DeviceName; };

	UFUNCTION(BlueprintPure, Category = "Ukaton Mission")
	uint8 GetBatteryLevel() const { return BatteryLevel; };

	UPROPERTY(BlueprintAssignable, Category = "Ukaton Mission")
	FBatteryLevelUpdatedDelegate OnBatteryLevelUpdated;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ukaton Mission")
	FUkatonSensorDataConfigurationsManager SensorDataConfigurationsManager;

	UPROPERTY(BlueprintReadOnly, Category = "Ukaton Mission")
	FUkatonSensorDataManager SensorDataManager;

	FUkatonHapticsManager HapticsManager;

	UPROPERTY(BlueprintReadOnly, Category = "Ukaton Mission")
	bool bIsConnected = false;

	virtual void Connect(const FString &DeviceIdentifier);
	void Connect();
	virtual void Disconnect();

	UPROPERTY(BlueprintAssignable, Category = "Ukaton Mission")
	FDelegateWithNoParams OnConnected;

	UPROPERTY(BlueprintAssignable, Category = "Ukaton Mission")
	FDelegateWithNoParams OnDisconnected;

	UPROPERTY(EditAnywhere, Category = "Ukaton Mission")
	bool bAutoConnect;

	UPROPERTY(EditAnywhere, Category = "Ukaton Mission")
	bool bRotateActor;

	UFUNCTION(BlueprintPure, Category = "Ukaton Mission")
	virtual FString GetAutoConnectDeviceIdentifier() const;

protected:
	// Called when the game starts or when spawned
	void BeginPlay() override;

	void ParseBatteryLevel(const TArray<uint8> &Data, uint8 &Offset);
	void ParseDeviceType(const TArray<uint8> &Data, uint8 &Offset);
	void ParseDeviceName(const TArray<uint8> &Data, uint8 &Offset);
	void ParseSensorData(const TArray<uint8> &Data, uint8 &Offset);

private:
	EUkatonDeviceType DeviceType;
	void UpdateDeviceType(const EUkatonDeviceType NewDeviceType);

	FString DeviceName;
	void UpdateDeviceName(const FString &NewDeviceName);

	uint8 BatteryLevel;
	void UpdateBatteryLevel(const uint8 NewBatteryLevel);

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
