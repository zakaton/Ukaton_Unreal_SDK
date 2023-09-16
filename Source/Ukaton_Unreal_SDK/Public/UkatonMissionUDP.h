// Copyright (c) 2023 Zack Qattan

#pragma once

#include "CoreMinimal.h"
#include "UkatonMissionBase.h"
#include "UkatonMissionUDP.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(UkatonMissionUDP, Log, All);

UCLASS()
class UKATON_UNREAL_SDK_API AUkatonMissionUDP : public AUkatonMissionBase
{
	GENERATED_BODY()

public:
	AUkatonMissionUDP();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Ukaton Mission UDP")
	void Connect(const FString &IPAddress) override;

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Ukaton Mission UDP")
	void Disconnect() override;
	UFUNCTION(BlueprintCallable, Category = "Ukaton Mission UDP")
	void Disconnect_Implementation();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Ukaton Mission UDP")
	void EmitBytes(const TArray<uint8> &Data);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ukaton Mission UDP", DisplayName = "IP Address")
	FString AutoConnectDeviceIPAddress = "0.0.0.0";

	UFUNCTION(BlueprintCallable, Category = "Ukaton Mission UDP")
	void ParseMessage(const TArray<uint8> &Data);

	virtual FString GetAutoConnectDeviceIdentifier() const override
	{
		return AutoConnectDeviceIPAddress;
	};

	UFUNCTION(BlueprintPure, Category = "Ukaton Mission UDP")
	static const int32 GetSendPort()
	{
		return SendPort;
	};

	UFUNCTION(BlueprintPure, Category = "Ukaton Mission UDP")
	const TArray<uint8> &GetPingMessage()
	{
		return bDidSendSetInListenPortMessage ? PingMessage : SetInListenPortMessage;
	};

	UFUNCTION(BlueprintPure, Category = "Ukaton Mission UDP")
	int32 GetInListenPort() { return InListenPort; };

	UFUNCTION(BlueprintCallable, Category = "Ukaton Mission UDP")
	UPARAM(DisplayName = "In Listen Port")
	int32 SetInListenPort(int32 NewInListenPort);

private:
	static const int32 SendPort = 9999;

	int32 InListenPort;

	static TArray<uint8> PingMessage;
	static TArray<uint8> RequestInfoMessage;

	TArray<uint8> SetInListenPortMessage;

	bool bDidSendSetInListenPortMessage;
	bool bDidReceiveDeviceInfo;
};
