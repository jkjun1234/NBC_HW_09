// HW_GameStateBase.cpp

#include "Game/HW_GameStateBase.h"

#include "Kismet/GameplayStatics.h"
#include "Player/HW_PlayerController.h"

void AHW_GameStateBase::MulticastRPCBroadcastLoginMSG_Implementation(const FString& InNameString)
{
	if (HasAuthority() == false)	// 서버가 아닌경우 ( 클라 )
	{
		APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		if (IsValid(PC) == true)
		{
			AHW_PlayerController* HWPC = Cast<AHW_PlayerController>(PC);
			if (IsValid(HWPC) == true)
			{
				FString NotificationString = InNameString + TEXT(" has Joined the game.");
				HWPC->PrintChatMessageString(NotificationString);	// Name과 HasJoin 문자열 출력 모든 클라이언트에게 보여줌
			}
		}
	}
}
