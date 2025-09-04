// GameStateBase.h
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "HW_GameStateBase.generated.h"

/**
 * 
 */
UCLASS()
class HW_09_API AHW_GameStateBase : public AGameStateBase
{
	GENERATED_BODY()

public:
	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPCBroadcastLoginMSG(const FString& InNameString = FString(TEXT("XXXXXXX")));
};
