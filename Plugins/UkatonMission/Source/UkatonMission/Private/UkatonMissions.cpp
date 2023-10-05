// Copyright (c) 2023 Zack Qattan

#include "UkatonMissions.h"
#include "Logging/StructuredLog.h"

DEFINE_LOG_CATEGORY(LogUkatonMissions);

AUkatonMissions::AUkatonMissions()
{
	PrimaryActorTick.bCanEverTick = false;
	for (uint8 SideIndex = 0; SideIndex < static_cast<uint8>(EUkatonMissionSide::COUNT); SideIndex++)
	{
		auto Side = static_cast<EUkatonMissionSide>(SideIndex);
		Masses.Emplace(Side, 0);
	}
}

void AUkatonMissions::BeginPlay()
{
	Super::BeginPlay();
}

void AUkatonMissions::SetMission(AUkatonMissionBase *NewUkatonMission, EUkatonMissionSide Side)
{
	UE_LOGFMT(LogUkatonMissions, Log, "Setting {0} Mission", UEnum::GetValueAsString(Side));
	auto UkatonMission = Insoles[Side];
	if (UkatonMission)
	{
		UnbindEventsFromMission(UkatonMission);
	}
	Insoles.Emplace(Side, NewUkatonMission);
	BindEventsToMission(NewUkatonMission);
}

void AUkatonMissions::SetLeftMission(AUkatonMissionBase *NewLeftMission)
{
	UE_LOGFMT(LogUkatonMissions, Log, "Setting Left Mission");
	SetMission(NewLeftMission, EUkatonMissionSide::LEFT);
}
void AUkatonMissions::SetRightMission(AUkatonMissionBase *NewRightMission)
{
	UE_LOGFMT(LogUkatonMissions, Log, "Setting Right Mission");
	SetMission(NewRightMission, EUkatonMissionSide::RIGHT);
}

void AUkatonMissions::BindEventsToMission(AUkatonMissionBase *UkatonMission)
{
	UE_LOGFMT(LogUkatonMissions, Log, "Binding Events to Mission...");
	UkatonMission->OnPressureValuesUpdated.AddDynamic(this, &AUkatonMissions::UpdatePressureData);
}
void AUkatonMissions::UnbindEventsFromMission(AUkatonMissionBase *UkatonMission)
{
	UE_LOGFMT(LogUkatonMissions, Log, "Unbinding Events from Mission...");
	UkatonMission->OnPressureValuesUpdated.RemoveDynamic(this, &AUkatonMissions::UpdatePressureData);
}

void AUkatonMissions::UpdatePressureData(const TArray<FUkatonPressureValue> &PressureValues, const int64 &Timestamp)
{
	UE_LOGFMT(LogUkatonMissions, Log, "Checking whether to UpdatePressureData...");
	if (IsConnected())
	{
		UE_LOGFMT(LogUkatonMissions, Log, "UpdatePressureDataa");

		RawValueSum = 0;
		CenterOfMass.Set(0, 0);

		for (uint8 SideIndex = 0; SideIndex < static_cast<uint8>(EUkatonMissionSide::COUNT); SideIndex++)
		{
			auto Side = static_cast<EUkatonMissionSide>(SideIndex);
			auto UkatonMission = Insoles[Side];
			auto &PressureData = UkatonMission->SensorDataManager.PressureData;
			RawValueSum += PressureData.PressureValuesWrapper.RawValueSum;
		}

		UE_LOGFMT(LogUkatonMissions, Log, "RawValueSum: {0}", RawValueSum);

		if (RawValueSum > 0)
		{
			for (uint8 SideIndex = 0; SideIndex < static_cast<uint8>(EUkatonMissionSide::COUNT); SideIndex++)
			{
				auto Side = static_cast<EUkatonMissionSide>(SideIndex);
				auto UkatonMission = Insoles[Side];
				auto &PressureData = UkatonMission->SensorDataManager.PressureData;
				Masses.Emplace(Side, PressureData.PressureValuesWrapper.RawValueSum / RawValueSum);
				UE_LOGFMT(LogUkatonMissions, Log, "{0} Mission Mass: {1}", UEnum::GetValueAsString(Side), Masses[Side]);
				CenterOfMass.Y += PressureData.CenterOfMass.Y * Masses[Side];
			}
			CenterOfMass.X = Masses[EUkatonMissionSide::RIGHT];

			UE_LOGFMT(LogUkatonMissions, Log, "CenterOfMass: {0}", *CenterOfMass.ToString());
			OnPressureCenterOfMassUpdated.Broadcast(CenterOfMass, Timestamp);
		}
	}
}

bool AUkatonMissions::IsConnected()
{
	bool bIsConnected = true;
	for (uint8 SideIndex = 0; bIsConnected && SideIndex < static_cast<uint8>(EUkatonMissionSide::COUNT); SideIndex++)
	{
		auto Side = static_cast<EUkatonMissionSide>(SideIndex);
		auto UkatonMission = Insoles[Side];
		if (UkatonMission)
		{
			bIsConnected = UkatonMission->GetIsConnected();
		}
		else
		{
			bIsConnected = false;
		}
	}
	return bIsConnected;
}