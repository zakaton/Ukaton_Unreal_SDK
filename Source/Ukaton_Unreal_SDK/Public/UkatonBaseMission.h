// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UkatonDeviceType.h"
#include "UkatonMotionData.h"
#include "UkatonPressureData.h"
#include "UkatonBaseMission.generated.h"

UCLASS()
class UKATON_UNREAL_SDK_API AUkatonBaseMission : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AUkatonBaseMission();

	UPROPERTY(BlueprintReadOnly, Category = "Ukaton")
	EUkatonDeviceType DeviceType;

	UPROPERTY(BlueprintReadOnly, Category = "Ukaton")
	bool bIsConnected;

	UPROPERTY(BlueprintReadOnly, Category = "Ukaton")
	FString DeviceName;

	UPROPERTY(BlueprintReadOnly, Category = "Ukaton")
	FUkatonMotionData UkatonMotionData;

	UPROPERTY(BlueprintReadOnly, Category = "Ukaton")
	FUkatonPressureData UkatonPressureData;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	FQuat TempQuaternion;
};
