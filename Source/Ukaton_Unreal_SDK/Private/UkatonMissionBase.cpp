// Copyright (c) 2023 Zack Qattan

#include "UkatonMissionBase.h"
#include "Logging/StructuredLog.h"

DEFINE_LOG_CATEGORY(UkatonMissionBase);

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
	if (NewDeviceType != DeviceType)
	{
		DeviceType = NewDeviceType;
		SensorDataConfigurationsManager.UpdateDeviceType(DeviceType);
		SensorDataManager.UpdateDeviceType(DeviceType);
	}
}

void AUkatonMissionBase::UpdateDeviceName(const FString &NewDeviceName)
{
	DeviceName = NewDeviceName;
}

void AUkatonMissionBase::UpdateBatteryLevel(const uint8 NewBatteryLevel)
{
	BatteryLevel = NewBatteryLevel;
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
	for (uint8 i = 0; i <= NameLength; i++)
	{
		NewName.AppendChar(static_cast<TCHAR>(Data[Offset++]));
	}
	UpdateDeviceName(NewName);
}

void AUkatonMissionBase::ParseSensorData(const TArray<uint8> &Data, uint8 &Offset)
{
	SensorDataManager.ParseSensorData(Data, Offset);
	// FILL - check flags and trigger events
}