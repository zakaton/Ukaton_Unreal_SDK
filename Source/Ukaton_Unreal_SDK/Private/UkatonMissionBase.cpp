// Copyright (c) 2023 Zack Qattan

#include "UkatonMissionBase.h"
#include "Logging/StructuredLog.h"

DEFINE_LOG_CATEGORY(LogUkatonMissionBase);

// Sets default values
AUkatonMissionBase::AUkatonMissionBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void AUkatonMissionBase::BeginPlay()
{
	Super::BeginPlay();

	if (bAutoConnect)
	{
		Connect();
	}
}

FString AUkatonMissionBase::GetAutoConnectDeviceIdentifier() const
{
	return TEXT("Default Device Identifier");
};

// Called every frame
void AUkatonMissionBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AUkatonMissionBase::UpdateDeviceType(const EUkatonDeviceType NewDeviceType)
{
	if (!bDidReceiveDeviceType || NewDeviceType != DeviceType)
	{
		DeviceType = NewDeviceType;
		UE_LOGFMT(LogUkatonMissionBase, Log, "DeviceType: {0}", UEnum::GetValueAsString(DeviceType));
		SensorDataConfigurationsManager.UpdateDeviceType(DeviceType);
		SensorDataManager.UpdateDeviceType(DeviceType);
		bDidReceiveDeviceType = true;
		OnDeviceInformationUpdate();
	}
}

void AUkatonMissionBase::UpdateDeviceName(const FString &NewDeviceName)
{
	DeviceName = NewDeviceName;
	UE_LOGFMT(LogUkatonMissionBase, Log, "DeviceName: {0}", DeviceName);
	bDidReceiveDeviceName = true;
	OnDeviceInformationUpdate();
}

void AUkatonMissionBase::OnDeviceInformationUpdate()
{
	if (bDidReceiveDeviceType && bDidReceiveDeviceName)
	{
		SetIsConnected(true);
	}
}

void AUkatonMissionBase::UpdateBatteryLevel(const uint8 NewBatteryLevel)
{
	BatteryLevel = NewBatteryLevel;
	UE_LOGFMT(LogUkatonMissionBase, Log, "BatteryLevel: {0}", BatteryLevel);
	OnBatteryLevelUpdated.Broadcast(NewBatteryLevel);
}

void AUkatonMissionBase::Connect(const FString &DeviceIdentifier)
{
}
void AUkatonMissionBase::Connect()
{
	Connect(GetAutoConnectDeviceIdentifier());
}
void AUkatonMissionBase::Disconnect()
{
	bDidReceiveDeviceName = false;
	bDidReceiveDeviceType = false;
	SetIsConnected(false);
}

void AUkatonMissionBase::ParseBatteryLevel(const TArray<uint8> &Data, uint8 &Offset)
{
	auto NewBatteryLevel = Data[Offset++];
	UpdateBatteryLevel(NewBatteryLevel);
}

void AUkatonMissionBase::ParseDeviceType(const TArray<uint8> &Data, uint8 &Offset)
{
	auto NewDeviceType = (EUkatonDeviceType)Data[Offset++];

	UpdateDeviceType(NewDeviceType);
}

void AUkatonMissionBase::ParseDeviceName(const TArray<uint8> &Data, uint8 &Offset)
{
	auto NameLength = Data[Offset++];
	FString NewName;
	NewName.Empty(NameLength);
	for (uint8 i = 0; i < NameLength; i++)
	{
		NewName.AppendChar(static_cast<TCHAR>(Data[Offset++]));
	}
	UpdateDeviceName(NewName);
}

void AUkatonMissionBase::ParseSensorData(const TArray<uint8> &Data, uint8 &Offset)
{
	SensorDataManager.ParseSensorData(Data, Offset);
	SensorDataManager.MotionData.DataUpdateFlags.IterateSetFlags([this](EUkatonMotionDataType MotionDataType)
																 { OnMotionDataUpdate(MotionDataType); },
																 true);
	SensorDataManager.PressureData.DataUpdateFlags.IterateSetFlags([this](EUkatonPressureDataType PressureDataType)
																   { OnPressureDataUpdate(PressureDataType); },
																   true);
}

void AUkatonMissionBase::OnMotionDataUpdate(EUkatonMotionDataType MotionDataType)
{
	switch (MotionDataType)
	{
	case EUkatonMotionDataType::ACCELERATION:
		OnAccelerationUpdated.Broadcast(SensorDataManager.MotionData.Acceleration, SensorDataManager.Timestamp);
		break;
	case EUkatonMotionDataType::GRAVITY:
		OnGravityUpdated.Broadcast(SensorDataManager.MotionData.Gravity, SensorDataManager.Timestamp);
		break;
	case EUkatonMotionDataType::LINEAR_ACCELERATION:
		OnLinearAccelerationUpdated.Broadcast(SensorDataManager.MotionData.LinearAcceleration, SensorDataManager.Timestamp);
		break;
	case EUkatonMotionDataType::ROTATION_RATE:
		OnRotationRateUpdated.Broadcast(SensorDataManager.MotionData.RotationRate, SensorDataManager.Timestamp);
		break;
	case EUkatonMotionDataType::MAGNETOMETER:
		OnMagnetometerUpdated.Broadcast(SensorDataManager.MotionData.Magnetometer, SensorDataManager.Timestamp);
		break;
	case EUkatonMotionDataType::QUATERNION:
		if (bRotateActor)
		{
			UE_LOGFMT(LogUkatonMissionBase, Log, "Rotating Actor {0}", *SensorDataManager.MotionData.Quaternion.ToString());
			SetActorRelativeRotation(SensorDataManager.MotionData.Quaternion);
		}
		OnQuaternionUpdated.Broadcast(SensorDataManager.MotionData.Quaternion, SensorDataManager.Timestamp);
		OnEulerUpdated.Broadcast(SensorDataManager.MotionData.Euler, SensorDataManager.Timestamp);
		break;
	default:
		UE_LOGFMT(LogUkatonMissionBase, Error, "Uncaught handler for MotionDataType: {0}", static_cast<uint8>(MotionDataType));
		break;
	}
}
void AUkatonMissionBase::OnPressureDataUpdate(EUkatonPressureDataType PressureDataType)
{
	switch (PressureDataType)
	{
	case EUkatonPressureDataType::PRESSURE_SINGLE_BYTE:
	case EUkatonPressureDataType::PRESSURE_DOUBLE_BYTE:
		OnPressureValuesUpdated.Broadcast(SensorDataManager.PressureData.PressureValuesWrapper.PressureValues, SensorDataManager.Timestamp);
		break;
	case EUkatonPressureDataType::CENTER_OF_MASS:
		OnPressureCenterOfMassUpdated.Broadcast(SensorDataManager.PressureData.CenterOfMass, SensorDataManager.Timestamp);
		break;
	case EUkatonPressureDataType::MASS:
		OnPressureMassUpdated.Broadcast(SensorDataManager.PressureData.Mass, SensorDataManager.Timestamp);
		break;
	case EUkatonPressureDataType::HEEL_TO_TOE:
		OnPressureHeelToToeUpdated.Broadcast(SensorDataManager.PressureData.HeelToToe, SensorDataManager.Timestamp);
		break;
	default:
		UE_LOGFMT(LogUkatonMissionBase, Error, "Uncaught handler for PressureDataType: {0}", static_cast<uint8>(PressureDataType));
		break;
	}
}

void AUkatonMissionBase::ParseMotionCalibration(const TArray<uint8> &Data, uint8 &Offset)
{
	SensorDataManager.MotionData.ParseCalibration(Data, Offset);
}

void AUkatonMissionBase::SetIsConnected(bool bNewIsConnected)
{
	if (bNewIsConnected != bIsConnected)
	{
		bIsConnected = bNewIsConnected;
		UE_LOGFMT(LogUkatonMissionBase, Log, "bIsConnected? {0}", bIsConnected);
		if (bIsConnected)
		{
			OnConnected.Broadcast();
			SetSensorDataConfigurations();
		}
		else
		{
			OnDisconnected.Broadcast();
		}
	}
}

void AUkatonMissionBase::SetSensorDataConfigurations()
{
	SensorDataConfigurationsManager.SerializeConfigurations();
}

void AUkatonMissionBase::RequestVibration(const TArray<uint8> &Vibration)
{
}

void AUkatonMissionBase::VibrateWaveforms(const TArray<EUkatonVibrationWaveformType> &Waveforms)
{
	// FILL
}
void AUkatonMissionBase::VibrateSequence(const TArray<FUkatonVibrationSequenceSegment> &Sequence)
{
	// FILL
}