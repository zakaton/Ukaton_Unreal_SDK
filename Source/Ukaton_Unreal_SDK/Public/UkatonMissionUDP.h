// Copyright (c) 2023 Zack Qattan

#pragma once

#include "CoreMinimal.h"
#include "UkatonMissionBase.h"
#include "UkatonMissionUDP.generated.h"

UCLASS()
class UKATON_UNREAL_SDK_API AUkatonMissionUDP : public AUkatonMissionBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Ukaton Mission")
	virtual void Connect(const FString &IPAddress) override;

	UFUNCTION(BlueprintCallable, Category = "Ukaton Mission")
	virtual void Disconnect() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ukaton Mission")
	FString DeviceIPAddress = "0.0.0.0";

	UFUNCTION(BlueprintCallable, Category = "Ukaton Mission UDP")
	void ParseMessage(TArray<uint8> Data);

	virtual FString GetAutoConnectDeviceIdentifier() const override;
};
