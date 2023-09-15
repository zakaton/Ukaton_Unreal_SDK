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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ukaton Mission UDP")
	FString DeviceIPAddress = "0.0.0.0";

	UFUNCTION(BlueprintCallable, Category = "Ukaton Mission UDP")
	void ParseMessage(const TArray<uint8> &Data);

	virtual FString GetAutoConnectDeviceIdentifier() const override;

	UFUNCTION(BlueprintPure, Category = "Ukaton Mission UDP")
	static const int32 GetSendPort()
	{
		return SendPort;
	};

private:
	static const int32 SendPort = 9999;
};
