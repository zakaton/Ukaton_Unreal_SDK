// Copyright (c) 2023 Zack Qattan

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Math/Vector2D.h"
#include "UkatonMissionBase.h"
#include "UkatonMissionSide.h"
#include "UkatonMissions.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogUkatonMissions, Log, All);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FPressureCenterOfMassDelegate, const FVector2D &, CenterOfMass, const int64 &, Timestamp);

UCLASS()
class UKATONMISSION_API AUkatonMissions : public AActor
{
	GENERATED_BODY()

public:
	AUkatonMissions();

	void SetMission(AUkatonMissionBase *NewUkatonMission, EUkatonMissionSide Side);

	UFUNCTION(BlueprintCallable, Category = "Ukaton Missions")
	void SetLeftMission(AUkatonMissionBase *NewLeftMission);
	UFUNCTION(BlueprintCallable, Category = "Ukaton Missions")
	void SetRightMission(AUkatonMissionBase *NewRightMission);

	UFUNCTION(BlueprintCallable, Category = "Ukaton Missions")
	void SetMissions(AUkatonMissionBase *NewLeftMission, AUkatonMissionBase *NewRightMission)
	{
		SetLeftMission(NewLeftMission);
		SetRightMission(NewRightMission);
	}

	void BindEventsToMission(AUkatonMissionBase *UkatonMission);
	void UnbindEventsFromMission(AUkatonMissionBase *UkatonMission);

	void UpdatePressureData(const TArray<FUkatonPressureValue> &PressureValues, const int64 &Timestamp);

	UFUNCTION(BlueprintPure, Category = "Ukaton Missions")
	bool IsConnected();

	UPROPERTY(BlueprintAssignable, Category = "Ukaton Missions")
	FPressureCenterOfMassDelegate OnPressureCenterOfMassUpdated;

protected:
	virtual void BeginPlay() override;

	TMap<EUkatonMissionSide, AUkatonMissionBase *> Insoles;

	float RawValueSum;
	FVector2D CenterOfMass;
	TMap<EUkatonMissionSide, float> Masses;
};
