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

void AUkatonMissionBase::UpdateDeviceType(EUkatonDeviceType NewDeviceType)
{
	if (NewDeviceType != DeviceType)
	{
		DeviceType = NewDeviceType;
		SensorDataConfigurationsManager.UpdateDeviceType(DeviceType);
		SensorDataManager.UpdateDeviceType(DeviceType);
	}
}

void AUkatonMissionBase::UpdateDeviceName(FString NewDeviceName)
{
	DeviceName = NewDeviceName;
}

void AUkatonMissionBase::UpdateBatteryLevel(uint8 NewBatteryLevel)
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