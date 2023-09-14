// Copyright (c) 2023 Zack Qattan

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UkatonDeviceType.h"
#include "UkatonSensorDataManager.h"
#include "UkatonSensorDataConfigurationsManager.h"
#include "UkatonHapticsManager.h"
#include "UkatonMissionBase.generated.h"

UCLASS()
class UKATON_UNREAL_SDK_API AUkatonMissionBase : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AUkatonMissionBase();

	UPROPERTY(BlueprintReadOnly, Category = "Ukaton")
	EUkatonDeviceType DeviceType;

	UPROPERTY(BlueprintReadOnly, Category = "Ukaton")
	bool bIsConnected;

	UPROPERTY(BlueprintReadOnly, Category = "Ukaton")
	FString DeviceName;

	UPROPERTY(BlueprintReadOnly, Category = "Ukaton")
	FUkatonSensorDataManager UkatonSensorDataManager;

	UPROPERTY(BlueprintReadOnly, Category = "Ukaton")
	FUkatonSensorDataConfigurationsManager UkatonSensorDataConfigurationsManager;

	FUkatonHapticsManager UkatonHapticsManager;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
