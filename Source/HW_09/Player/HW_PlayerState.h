// 
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "HW_PlayerState.generated.h"

/**
 * 
 */
UCLASS()
class HW_09_API AHW_PlayerState : public APlayerState
{
	GENERATED_BODY()
public:
	AHW_PlayerState();
	// 리플리케이션 사용을 위한 함수
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
public:
	UPROPERTY(Replicated)
	FString PlayerNameStr;

	UPROPERTY(Replicated)
	int32 CurrentGuessCount;	// 현재 추측 횟수
	
	int32 MaxGuessCount;	//최대 추측 횟수 전부 공통으로 차이가 없을 예정으로 리플리케이션 하지않음

	FString GetPlayerInfoStr();
};
