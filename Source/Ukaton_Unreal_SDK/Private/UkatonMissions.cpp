// Copyright (c) 2023 Zack Qattan

#include "UkatonMissions.h"

AUkatonMissions::AUkatonMissions()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AUkatonMissions::BeginPlay()
{
	Super::BeginPlay();
}

void AUkatonMissions::SetMission(AUkatonMissionBase *NewUkatonMission, EUkatonMissionSide Side)
{
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
	SetMission(NewLeftMission, EUkatonMissionSide::LEFT);
}
void AUkatonMissions::SetRightMission(AUkatonMissionBase *NewRightMission)
{
	SetMission(NewRightMission, EUkatonMissionSide::RIGHT);
}

void AUkatonMissions::BindEventsToMission(AUkatonMissionBase *UkatonMission)
{
	UkatonMission->OnPressureValuesUpdated.AddDynamic(this, &AUkatonMissions::UpdatePressureData);
}
void AUkatonMissions::UnbindEventsFromMission(AUkatonMissionBase *UkatonMission)
{
	UkatonMission->OnPressureValuesUpdated.RemoveDynamic(this, &AUkatonMissions::UpdatePressureData);
}

void AUkatonMissions::UpdatePressureData(const TArray<FUkatonPressureValue> &PressureValues, const int64 &Timestamp)
{
	if (IsConnected())
	{
		// FILL
		for (uint8 SideIndex = 0; SideIndex < static_cast<uint8>(EUkatonMissionSide::COUNT); SideIndex++)
		{
			auto Side = static_cast<EUkatonMissionSide>(SideIndex);
			auto UkatonMission = Insoles[Side];
			// FILL
		}
		OnPressureCenterOfMassUpdated.Broadcast(CenterOfMass, Timestamp);
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