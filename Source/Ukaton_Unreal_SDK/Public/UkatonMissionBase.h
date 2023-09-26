// Copyright (c) 2023 Zack Qattan

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UkatonDeviceType.h"
#include "UkatonSensorDataConfigurationsManager.h"
#include "UkatonSensorDataManager.h"
#include "UkatonHapticsManager.h"
#include "UkatonVibrationWaveformType.h"
#include "UkatonVibrationSequenceSegment.h"
#include "UkatonMotionDataType.h"
#include "UkatonPressureDataType.h"
#include "UkatonMissionBase.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogUkatonMissionBase, Log, Warning);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDelegateWithNoParams);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FBatteryLevelUpdatedDelegate, const uint8, BatteryLevel);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FMotionVectorUpdatedDelegate, const FVector &, Vector, const int64, Timestamp);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FMotionEulerUpdatedDelegate, const FRotator &, Euler, const int64, Timestamp);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FMotionQuaternionUpdatedDelegate, const FQuat &, Quaternion, const int64, Timestamp);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FPressureValuesUpdatedDelegate, const TArray<FUkatonPressureValue> &, PressureValues, const int64, Timestamp);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FPressureCenterOfMassUpdatedDelegate, const FVector2D &, CenterOfMass, const int64, Timestamp);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FPressureMassUpdatedDelegate, const float &, Mass, const int64, Timestamp);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FPressureHeelToToeUpdatedDelegate, const double &, HeelToToe, const int64, Timestamp);

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

	UPROPERTY(BlueprintReadOnly, Category = "Ukaton Mission")
	FUkatonHapticsManager HapticsManager;

	UFUNCTION(BlueprintPure, Category = "Ukaton Mission")
	bool GetIsConnected() const { return bIsConnected; };

	virtual void Connect(const FString &DeviceIdentifier);
	void Connect();
	virtual void Disconnect();

	UPROPERTY(BlueprintAssignable, Category = "Ukaton Mission")
	FDelegateWithNoParams OnConnected;

	UPROPERTY(BlueprintAssignable, Category = "Ukaton Mission")
	FDelegateWithNoParams OnDisconnected;

	UPROPERTY(EditAnywhere, Category = "Ukaton Mission")
	bool bAutoConnect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ukaton Mission")
	bool bRotateActor;

	UPROPERTY(BlueprintAssignable, Category = "Ukaton Mission Motion Data")
	FMotionVectorUpdatedDelegate OnAccelerationUpdated;
	UPROPERTY(BlueprintAssignable, Category = "Ukaton Mission Motion Data")
	FMotionVectorUpdatedDelegate OnGravityUpdated;
	UPROPERTY(BlueprintAssignable, Category = "Ukaton Mission Motion Data")
	FMotionVectorUpdatedDelegate OnLinearAccelerationUpdated;
	UPROPERTY(BlueprintAssignable, Category = "Ukaton Mission Motion Data")
	FMotionEulerUpdatedDelegate OnRotationRateUpdated;
	UPROPERTY(BlueprintAssignable, Category = "Ukaton Mission Motion Data")
	FMotionVectorUpdatedDelegate OnMagnetometerUpdated;
	UPROPERTY(BlueprintAssignable, Category = "Ukaton Mission Motion Data")
	FMotionQuaternionUpdatedDelegate OnQuaternionUpdated;
	UPROPERTY(BlueprintAssignable, Category = "Ukaton Mission Motion Data")
	FMotionEulerUpdatedDelegate OnEulerUpdated;

	UPROPERTY(BlueprintAssignable, Category = "Ukaton Mission Pressure Data")
	FPressureValuesUpdatedDelegate OnPressureValuesUpdated;
	UPROPERTY(BlueprintAssignable, Category = "Ukaton Mission Pressure Data")
	FPressureCenterOfMassUpdatedDelegate OnPressureCenterOfMassUpdated;
	UPROPERTY(BlueprintAssignable, Category = "Ukaton Mission Pressure Data")
	FPressureMassUpdatedDelegate OnPressureMassUpdated;
	UPROPERTY(BlueprintAssignable, Category = "Ukaton Mission Pressure Data")
	FPressureHeelToToeUpdatedDelegate OnPressureHeelToToeUpdated;

	UFUNCTION(BlueprintCallable, Category = "Ukaton Mission")
	void VibrateWaveforms(const TArray<EUkatonVibrationWaveformType> &Waveforms);
	UFUNCTION(BlueprintCallable, Category = "Ukaton Mission")
	void VibrateSequence(const TArray<FUkatonVibrationSequenceSegment> &Sequence);

protected:
	// Called when the game starts or when spawned
	void BeginPlay() override;

	virtual void SetIsConnected(bool bNewIsConnected);

	void ParseBatteryLevel(const TArray<uint8> &Data, uint8 &Offset);
	void ParseDeviceType(const TArray<uint8> &Data, uint8 &Offset);
	void ParseDeviceName(const TArray<uint8> &Data, uint8 &Offset);
	void ParseSensorData(const TArray<uint8> &Data, uint8 &Offset);
	void ParseMotionCalibration(const TArray<uint8> &Data, uint8 &Offset);

	virtual void SetSensorDataConfigurations();

	void OnMotionDataUpdate(EUkatonMotionDataType MotionDataType);
	void OnPressureDataUpdate(EUkatonPressureDataType PressureDataType);

	virtual FString GetAutoConnectDeviceIdentifier() const;

	virtual void RequestVibration(const TArray<uint8> &Vibration);

protected:
	bool bIsConnected = false;

	EUkatonDeviceType DeviceType;
	void UpdateDeviceType(const EUkatonDeviceType NewDeviceType);

	FString DeviceName;
	void UpdateDeviceName(const FString &NewDeviceName);

	uint8 BatteryLevel;
	void UpdateBatteryLevel(const uint8 NewBatteryLevel);

	bool bDidReceiveDeviceName;
	bool bDidReceiveDeviceType;

	void OnDeviceInformationUpdate();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
